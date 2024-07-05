#include <napi.h>
#include <Windows.h>
#include <lm.h>
#include <string>

#pragma comment(lib, "netapi32.lib")

std::string GetPrivilege(const std::wstring& username) {
    USER_INFO_1* userInfo;
    NET_API_STATUS status = NetUserGetInfo(nullptr, username.c_str(), 1, (LPBYTE*)&userInfo);
    if (status != NERR_Success) {
        return "Пользователя нет";
    }
    std::string privilege = "Пользователь";
    if (userInfo->usri1_priv == USER_PRIV_ADMIN) {
        privilege = "Администратор";
    } else if (userInfo->usri1_priv == USER_PRIV_USER) {
        privilege = "Пользователь";
    } else if (userInfo->usri1_priv == USER_PRIV_GUEST) {
        privilege = "Гость";
    }
    NetApiBufferFree(userInfo);
    return privilege;
}

Napi::String CheckUserPrivilege(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a string").ThrowAsJavaScriptException();
    }
    std::string username = info[0].As<Napi::String>();
    std::wstring wUsername(username.begin(), username.end());
    std::string privilege = GetPrivilege(wUsername);
    return Napi::String::New(env, privilege);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkUserPrivilege"), Napi::Function::New(env, CheckUserPrivilege));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
