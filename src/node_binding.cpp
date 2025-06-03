#include <napi.h>
#include "formatter.h"

Napi::String FormatText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }
    
    std::string input = info[0].As<Napi::String>().Utf8Value();
    std::string result = Formatter::format_text(input);
    
    return Napi::String::New(env, result);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "formatText"), Napi::Function::New(env, FormatText));
    return exports;
}

NODE_API_MODULE(pyformat, Init)
