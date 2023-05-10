/**
 * @file app.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../simplekv/simplekv.h"


namespace MOONCAKE {


    /* Inherit and override "Life cycle" to create your own App */
    class APP_BASE {
        private:
            std::string _name;
            void* _user_data;
            void* _icon_addr;
            bool _allow_bg_running;
            bool _finished;
            SIMPLEKV::SimpleKV* _database;


        protected:
            /* App internal API */
            inline void setAppName(const std::string& name) { _name = name; }
            inline void setAppIcon(void* icon) { _icon_addr = icon; }
            inline void setAllowBgRunning(bool allow) { _allow_bg_running = allow; }
            inline void endApp() { _finished = true; }
            inline SIMPLEKV::SimpleKV* getDatabase() { return _database; }
            inline void* getUserData() { return _user_data; };


        public:
            APP_BASE() :
                _name(""),
                _user_data(nullptr),
                _icon_addr(nullptr),
                _allow_bg_running(false),
                _finished(false),
                _database(nullptr) {}
            ~APP_BASE() = default;


            /* API for App manager */
            inline void setDatabase(SIMPLEKV::SimpleKV* db) { _database = db; }
            inline void setUserData(void* userData) { _user_data = userData; }

            
            /* Basic API */
            inline std::string getAppName() { return _name; }
            inline void* getIconAddr() { return _icon_addr; }
            inline bool isAllowBgRunning() { return _allow_bg_running; }
            inline bool isFinished() { return _finished; }


            /**
             * @brief Lifecycle callbacks for derived to override
             * 
             */
            /* Setup App configs, called when App "install()" */
            virtual void onSetup() {}

            /* Life cycle */
            virtual void onCreate() {}
            virtual void onResume() {}
            virtual void onRunning() {}
            virtual void onRunningBG() {}
            virtual void onPause() {}
            virtual void onDestroy() {}

    };


}