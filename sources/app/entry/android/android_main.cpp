#include "Config_Platforms.h"
#if PLATFORMS_ANDROID
#include <jni.h>

#include "AndroidOut.h"
#include "Renderer.h"

#include <game-activity/GameActivity.cpp>
#include <game-text-input/gametextinput.cpp>
#include <android/imagedecoder.h>
#include <android/asset_manager.h>
#include <GLES3/gl3.h>
// //include <game-activity/native_app_glue/android_native_app_glue.h>
// void Renderer::handleInput() {
//     // handle all queued inputs
//     auto *inputBuffer = android_app_swap_input_buffers(app_);
//     if (!inputBuffer) {
//         // no inputs yet.
//         return;
//     }

//     // handle motion events (motionEventsCounts can be 0).
//     for (auto i = 0; i < inputBuffer->motionEventsCount; i++) {
//         auto &motionEvent = inputBuffer->motionEvents[i];
//         auto action = motionEvent.action;

//         // Find the pointer index, mask and bitshift to turn it into a readable value.
//         auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
//                 >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
//         aout << "Pointer(s): ";

//         // get the x and y position of this event if it is not ACTION_MOVE.
//         auto &pointer = motionEvent.pointers[pointerIndex];
//         auto x = GameActivityPointerAxes_getX(&pointer);
//         auto y = GameActivityPointerAxes_getY(&pointer);

//         // determine the action type and process the event accordingly.
//         switch (action & AMOTION_EVENT_ACTION_MASK) {
//             case AMOTION_EVENT_ACTION_DOWN:
//             case AMOTION_EVENT_ACTION_POINTER_DOWN:
//                 aout << "(" << pointer.id << ", " << x << ", " << y << ") "
//                      << "Pointer Down";
//                 break;

//             case AMOTION_EVENT_ACTION_CANCEL:
//                 // treat the CANCEL as an UP event: doing nothing in the app, except
//                 // removing the pointer from the cache if pointers are locally saved.
//                 // code pass through on purpose.
//             case AMOTION_EVENT_ACTION_UP:
//             case AMOTION_EVENT_ACTION_POINTER_UP:
//                 aout << "(" << pointer.id << ", " << x << ", " << y << ") "
//                      << "Pointer Up";
//                 break;

//             case AMOTION_EVENT_ACTION_MOVE:
//                 // There is no pointer index for ACTION_MOVE, only a snapshot of
//                 // all active pointers; app needs to cache previous active pointers
//                 // to figure out which ones are actually moved.
//                 for (auto index = 0; index < motionEvent.pointerCount; index++) {
//                     pointer = motionEvent.pointers[index];
//                     x = GameActivityPointerAxes_getX(&pointer);
//                     y = GameActivityPointerAxes_getY(&pointer);
//                     aout << "(" << pointer.id << ", " << x << ", " << y << ")";

//                     if (index != (motionEvent.pointerCount - 1)) aout << ",";
//                     aout << " ";
//                 }
//                 aout << "Pointer Move";
//                 break;
//             default:
//                 aout << "Unknown MotionEvent Action: " << action;
//         }
//         aout << std::endl;
//     }
//     // clear the motion input count in this buffer for main thread to re-use.
//     android_app_clear_motion_events(inputBuffer);

//     // handle input key events.
//     for (auto i = 0; i < inputBuffer->keyEventsCount; i++) {
//         auto &keyEvent = inputBuffer->keyEvents[i];
//         aout << "Key: " << keyEvent.keyCode <<" ";
//         switch (keyEvent.action) {
//             case AKEY_EVENT_ACTION_DOWN:
//                 aout << "Key Down";
//                 break;
//             case AKEY_EVENT_ACTION_UP:
//                 aout << "Key Up";
//                 break;
//             case AKEY_EVENT_ACTION_MULTIPLE:
//                 // Deprecated since Android API level 29.
//                 aout << "Multiple Key Actions";
//                 break;
//             default:
//                 aout << "Unknown KeyEvent Action: " << keyEvent.action;
//         }
//         aout << std::endl;
//     }
//     // clear the key input count too.
//     android_app_clear_key_events(inputBuffer);
// }
extern "C" {

#include <game-activity/native_app_glue/android_native_app_glue.c>

/*!
 * Handles commands sent to this Android application
 * @param pApp the app the commands are coming from
 * @param cmd the command to handle
 */
void handle_cmd(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            // A new window is created, associate a renderer with it. You may replace this with a
            // "game" class if that suits your needs. Remember to change all instances of userData
            // if you change the class here as a reinterpret_cast is dangerous this in the
            // android_main function and the APP_CMD_TERM_WINDOW handler case.
            pApp->userData = new Renderer(pApp);
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being destroyed. Use this to clean up your userData to avoid leaking
            // resources.
            //
            // We have to check if userData is assigned just in case this comes in really quickly
            if (pApp->userData) {
                //
                auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);
                pApp->userData = nullptr;
                delete pRenderer;
            }
            break;
        default:
            break;
    }
}

/*!
 * Enable the motion events you want to handle; not handled events are
 * passed back to OS for further processing. For this example case,
 * only pointer and joystick devices are enabled.
 *
 * @param motionEvent the newly arrived GameActivityMotionEvent.
 * @return true if the event is from a pointer or joystick device,
 *         false for all other input devices.
 */
bool motion_event_filter_func(const GameActivityMotionEvent *motionEvent) {
    auto sourceClass = motionEvent->source & AINPUT_SOURCE_CLASS_MASK;
    return (sourceClass == AINPUT_SOURCE_CLASS_POINTER ||
            sourceClass == AINPUT_SOURCE_CLASS_JOYSTICK);
}

/*!
 * This the main entry point for a native activity
 */
void android_main(struct android_app *pApp) {
    // Can be removed, useful to ensure your code is running
    aout << "Welcome to android_main" << std::endl;

    // Register an event handler for Android events
    pApp->onAppCmd = handle_cmd;

    // Set input event filters (set it to NULL if the app wants to process all inputs).
    // Note that for key inputs, this example uses the default default_key_filter()
    // implemented in android_native_app_glue.c.
    android_app_set_motion_event_filter(pApp, motion_event_filter_func);

    // This sets up a typical game/event loop. It will run until the app is destroyed.
    do {
        // Process all pending events before running game logic.
        bool done = false;
        while (!done) {
            // 0 is non-blocking.
            int timeout = 0;
            int events;
            android_poll_source *pSource;
            int result = ALooper_pollOnce(timeout, nullptr, &events,
                                          reinterpret_cast<void**>(&pSource));
            switch (result) {
                case ALOOPER_POLL_TIMEOUT:
                    [[clang::fallthrough]];
                case ALOOPER_POLL_WAKE:
                    // No events occurred before the timeout or explicit wake. Stop checking for events.
                    done = true;
                    break;
                case ALOOPER_EVENT_ERROR:
                    aout << "ALooper_pollOnce returned an error" << std::endl;
                    break;
                case ALOOPER_POLL_CALLBACK:
                    break;
                default:
                    if (pSource) {
                        pSource->process(pApp, pSource);
                    }
            }
        }

        // Check if any user data is associated. This is assigned in handle_cmd
        if (pApp->userData) {
            // We know that our user data is a Renderer, so reinterpret cast it. If you change your
            // user data remember to change it here
            auto *pRenderer = reinterpret_cast<Renderer *>(pApp->userData);

            // Process game input
            pRenderer->handleInput();

            // Render a frame
            pRenderer->render();
        }
    } while (!pApp->destroyRequested);
}
}
#endif