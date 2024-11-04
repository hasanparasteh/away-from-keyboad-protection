#include <iostream>
#include <chrono>
#include <string>

#ifdef __APPLE__

#include <ApplicationServices/ApplicationServices.h>
extern "C" void showAlert(const char*);
#endif

std::string sequenceBuffer;
auto lastKeyPressTime = std::chrono::steady_clock::now();
const int MAX_INTERVAL = 2000;
bool inputBlocked = false;
CGPoint lockedPosition = {500, 500};

void resetSequence() {
    sequenceBuffer.clear();
}


void checkKeySequence(char key) {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastKeyPressTime).count();

    if (duration > MAX_INTERVAL) {
        resetSequence();
    }


    sequenceBuffer += key;
    if (sequenceBuffer.length() > 4) {
        sequenceBuffer.erase(0, 1);
    }


    if (sequenceBuffer == "afka") {
        resetSequence();
        inputBlocked = !inputBlocked;

        if(inputBlocked) {
            showAlert("Everything is locked!");
        }
    }

    lastKeyPressTime = now;
}

#ifdef __APPLE__


CGEventRef keyMouseWatcherCallback(CGEventTapProxy, CGEventType type, CGEventRef event, [[maybe_unused]] void *refcon) {
    if (inputBlocked) {

        if (type == kCGEventKeyDown || type == kCGEventKeyUp ||
            type == kCGEventLeftMouseDown || type == kCGEventRightMouseDown ||
            type == kCGEventLeftMouseUp || type == kCGEventRightMouseUp ||
            type == kCGEventOtherMouseDown || type == kCGEventOtherMouseUp ||
            type == kCGEventScrollWheel || type == kCGEventMouseMoved) {

            if (type == kCGEventKeyDown) {
                auto keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);


                char key = '\0';
                switch (keycode) {
                    case 0:
                        key = 'a';
                        break;
                    case 3:
                        key = 'f';
                        break;
                    case 40:
                        key = 'k';
                        break;

                }

                if (key != '\0') {
                    checkKeySequence(key);
                }
            }


            if (type == kCGEventMouseMoved) {
                CGWarpMouseCursorPosition(lockedPosition);
            }

            return NULL;
        }
    } else {

        if (type == kCGEventKeyDown) {
            auto keycode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
            char key = '\0';
            switch (keycode) {
                case 0:
                    key = 'a';
                    break;
                case 3:
                    key = 'f';
                    break;
                case 40:
                    key = 'k';
                    break;
            }

            if (key != '\0') {
                checkKeySequence(key);
            }
        }
    }
    return event;
}

void startKeyMouseWatcherMacOS() {
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp) |
                            CGEventMaskBit(kCGEventLeftMouseDown) | CGEventMaskBit(kCGEventRightMouseDown) |
                            CGEventMaskBit(kCGEventLeftMouseUp) | CGEventMaskBit(kCGEventRightMouseUp) |
                            CGEventMaskBit(kCGEventMouseMoved) | CGEventMaskBit(kCGEventOtherMouseDown) |
                            CGEventMaskBit(kCGEventOtherMouseUp) | CGEventMaskBit(kCGEventScrollWheel);

    CFMachPortRef eventTap = CGEventTapCreate(
            kCGSessionEventTap,
            kCGHeadInsertEventTap,
            kCGEventTapOptionDefault,
            eventMask,
            keyMouseWatcherCallback,
            NULL
    );

    if (!eventTap) {
        std::cerr << "Failed to create event tap\n";
        exit(1);
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}

#endif

int main() {
#ifdef __APPLE__
    startKeyMouseWatcherMacOS();
#endif
    return 0;
}
