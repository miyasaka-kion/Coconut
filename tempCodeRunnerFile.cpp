#include <iostream>
#include <functional>

class CallbackManager {
public:
    using CallbackFunction = std::function<void(int)>;

    void RegisterCallback(CallbackFunction callback) {
        callback_ = callback;
    }

    void TriggerCallback(int value) {
        if (callback_) {
            callback_(value);
        } else {
            DefaultCallback(value);
        }
    }

    void DefaultCallback(int value) {
        std::cout << "Default Callback triggered with value: " << value << std::endl;
    }

private:
    CallbackFunction callback_;
};

class MyClass {
public:
    void MyCallback(int value) {
        std::cout << "Callback triggered with value: " << value << std::endl;
    }
};

int main() {
    CallbackManager callbackManager;
    MyClass myObject;

    // 注册回调函数
    callbackManager.RegisterCallback(std::bind(&MyClass::MyCallback, &myObject, std::placeholders::_1));

    // 触发回调函数
    callbackManager.TriggerCallback(42);

    // 没有注册回调函数时，使用默认回调函数
    callbackManager.RegisterCallback(nullptr);
    callbackManager.TriggerCallback(100);

    return 0;
}