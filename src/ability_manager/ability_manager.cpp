/**
 * @file ability_manager.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-09-26
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "ability_manager.h"
#include "ability/ability.h"

using namespace Mooncake;

int AbilityManager::createAbility(std::unique_ptr<AbilityBase> ability)
{
    if (!ability) {
        return -1;
    }

    // 分配 Ability ID
    AbilityInfo_t new_ability_info;
    new_ability_info.id = get_next_ability_id();
    new_ability_info.state = StateGoCreate;
    new_ability_info.ability = std::move(ability);

    // 转移所有权
    _ability_list.push_back(std::move(new_ability_info));

    return new_ability_info.id;
}

bool AbilityManager::destroyAbility(int abilityID)
{
    // 遍历查找对应 ID 的 Ability
    for (auto& ability_info : _ability_list) {
        if (ability_info.id == abilityID) {
            // 更新状态
            ability_info.state = StateGoDestroy;
            return true;
        }
    }
    return false;
}

void AbilityManager::updateAbilities()
{
    // 遍历所有 Abilities
    for (auto ability_iter = _ability_list.begin(); ability_iter != _ability_list.end();) {
        /* ----------------------------------- 状态机 ---------------------------------- */
        switch (ability_iter->state) {
            case StateGoCreate: {
                // 触发创建回调
                ability_iter->ability->baseCreate();
                // 切到正常刷新状态
                ability_iter->state = StateUpdating;
                ability_iter->ability->baseUpdate();
                ability_iter++;
                break;
            }
            case StateUpdating: {
                ability_iter->ability->baseUpdate();
                ability_iter++;
                break;
            }
            case StateGoDestroy: {
                // 触发销毁回调
                ability_iter->ability->baseDestroy();
                // 把 ID 存到可用 ID 列表，防止反复创建销毁导致的 ID 溢出
                _available_ability_id_list.push_back(ability_iter->id);
                // 销毁 Ability
                ability_iter = _ability_list.erase(ability_iter);
                break;
            }
            default: {
                ability_iter++;
                break;
            }
        }
    }
}

AbilityBase* AbilityManager::getAbilityInstance(int abilityID)
{
    // 遍历查找对应 ID 的 Ability
    for (auto& ability_info : _ability_list) {
        if (ability_info.id == abilityID) {
            return ability_info.ability.get();
        }
    }
    return nullptr;
}

AbilityType::Type_t AbilityManager::getAbilityType(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        return ability_instance->abilityType();
    }
    return AbilityType::Base;
}

bool AbilityManager::isAbilityExist(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        return true;
    }
    return false;
}

int AbilityManager::get_next_ability_id()
{
    int next_ability_id = -1;

    // 检查可用 ID 列表
    if (!_available_ability_id_list.empty()) {
        next_ability_id = _available_ability_id_list.front();
        _available_ability_id_list.erase(_available_ability_id_list.begin());
        return next_ability_id;
    }

    // 如果没有，继续自加
    next_ability_id = _next_ability_id;
    _next_ability_id++;

    return next_ability_id;
}

/* -------------------------------------------------------------------------- */
/*                             UI Ability API Wrap                            */
/* -------------------------------------------------------------------------- */

bool AbilityManager::showUIAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::UI) {
            static_cast<UIAbility*>(ability_instance)->show();
            return true;
        }
    }
    return false;
}

bool AbilityManager::hideUIAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::UI) {
            static_cast<UIAbility*>(ability_instance)->hide();
            return true;
        }
    }
    return false;
}

UIAbility::UIAbilityState_t AbilityManager::getUIAbilityCurrentState(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::UI) {
            return static_cast<UIAbility*>(ability_instance)->currentState();
        }
    }
    return UIAbility::StateNull;
}

/* -------------------------------------------------------------------------- */
/*                           Worker Ability API Wrap                          */
/* -------------------------------------------------------------------------- */

bool AbilityManager::pauseWorkerAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::Worker) {
            static_cast<WorkerAbility*>(ability_instance)->pause();
            return true;
        }
    }
    return false;
}

bool AbilityManager::resumeWorkerAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::Worker) {
            static_cast<WorkerAbility*>(ability_instance)->resume();
            return true;
        }
    }
    return false;
}

WorkerAbility::WorkerAbilityState_t AbilityManager::getWorkerAbilityCurrentState(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::Worker) {
            return static_cast<WorkerAbility*>(ability_instance)->currentState();
        }
    }
    return WorkerAbility::StateNull;
}

/* -------------------------------------------------------------------------- */
/*                            App Ability API Wrap                            */
/* -------------------------------------------------------------------------- */
bool AbilityManager::openAppAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::App) {
            static_cast<AppAbility*>(ability_instance)->open();
            return true;
        }
    }
    return false;
}

bool AbilityManager::closeAppAbility(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::App) {
            static_cast<AppAbility*>(ability_instance)->close();
            return true;
        }
    }
    return false;
}

AppAbility::AppInfo_t AbilityManager::getAppAbilityAppInfo(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::App) {
            return static_cast<AppAbility*>(ability_instance)->getAppInfo();
        }
    }
    return AppAbility::AppInfo_t();
}

AppAbility::AppAbilityState_t AbilityManager::getAppAbilityCurrentState(int abilityID)
{
    auto ability_instance = getAbilityInstance(abilityID);
    if (ability_instance) {
        // 类型校验
        if (ability_instance->abilityType() == AbilityType::App) {
            return static_cast<AppAbility*>(ability_instance)->currentState();
        }
    }
    return AppAbility::StateNull;
}
