//
// Created by Akira Mujawar on 06/07/22.
//

#include "sdl_wrapper.hpp"
#include "application.hpp"

#include "imgui_wrapper.hpp"
#include "log.hpp"

using physicat::Application;

namespace {
    #ifdef EMSCRIPTEN
        // Free Function in an anonymous namespace private to this file
        void EmscriptenLoop(physicat::Application* engine) {
            engine->LoopApplication();
        }
    #endif
} // namespace

struct Application::Internal {
    const float FramePerSecond; // PerformanceFrequency
    const float FixedTimeStep;
    uint64_t CurrentFrameTime;
    uint64_t PreviousFrameTime;
    float Accumulator; // For fixed update
    float CurrentDeltaTime;

    Internal()
        : FramePerSecond(static_cast<float>(SDL_GetPerformanceFrequency()))
        , FixedTimeStep(1 / 60.0f)
        , CurrentFrameTime(SDL_GetPerformanceCounter())
        , PreviousFrameTime(CurrentFrameTime)
         {}

    float TimeStep() {
        PT_PROFILE_SCOPE;
        PreviousFrameTime = CurrentFrameTime;
        CurrentFrameTime = SDL_GetPerformanceCounter();

        float elapsedTime {
            (CurrentFrameTime - PreviousFrameTime) * 1000.0f
        };

        float deltaTime = (elapsedTime / FramePerSecond) * 0.001f;

        Accumulator += deltaTime;
        CurrentDeltaTime = deltaTime;

        return deltaTime;
    }

    bool FixedStep() {
        if(Accumulator > FixedTimeStep) {
            Accumulator -= FixedTimeStep;
            return true;
        }

        return false;
    }
};

void physicat::Application::StartApplication() {
    PT_PROFILE_SCOPE;
    physicat::Log("physicat::Application::StartApplication", "Starting Application");

    #ifdef __EMSCRIPTEN__
        //  emscripten_set_main_loop(emscriptenLoop, 60, 1);
        emscripten_set_main_loop_arg((em_arg_callback_func) ::EmscriptenLoop, this, 60, 1);
    #else
//    double targetFrameTime = 1.0 / 60.0;  // Targeting 60 FPS
//    double frameStartTime = SDL_GetTicks();
//    // Manually control frame timing
//    double frameEndTime = GetTime();
//    double frameDuration = frameEndTime - frameStartTime;
//
//    if (frameDuration < targetFrameTime) {
//        Sleep(targetFrameTime - frameDuration);
//    }

//        PhysicsThread = std::thread(&physicat::Application::PhysicsLoop, this);


//        while (LoopApplication()) {
//            // Just waiting for the main loop to end.
//        }

        IsApplicationRunning = true;

        RenderThread = std::thread(&physicat::Application::RenderLoop, this);

        while(IsApplicationRunning)
        {
//            physicat::Log("Main Thread", "Running");
//            float deltaTime = InternalPointer->TimeStep();
//            FpsCounter.frameStart();
//            // If Input() returns false - close the application
//            if(!Input(deltaTime)) {
//                IsApplicationRunning = false;
//            }
//
//            if(InternalPointer->FixedStep()) {
//                FixedUpdate(InternalPointer->Accumulator + InternalPointer->FixedTimeStep);
//            }
//
//            Update(deltaTime);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

//        IsApplicationRunning = false;
//        PhysicsThread.join();
        RenderThread.join();



    #endif
}

// state based sync
// keep enqueing the changes -- nnooo

// one was
// -- event based double buffering -- for UI input to game thread
// -- dirty flag based double buffering -- useless kinda too expensive
// -- queue based double buffering -- best for positions and rotations

/**
 * class Component {
 *      int render_buffer_x;
 *      int main_buffer_x;
 *
 *      public bool isChanged = false;
 *
 *      public void UpdateFromUIOnRender()
 *      {
 *          render_buffer_x = 10;
 *      }
 *
 *      public void Sync()
 *      {
 *          if(isChanged)
 *          {
 *              main_buffer_x = render_buffer_x;
 *              isChanged = false;
 *          }
 *      }
 * }
 *
 *
 *  class Component {
 *      int value;
 *
 *      queue<int> valueUpdateFromRender; // there will be always a overhead...
 *
 *      UpdateValue() {
 *
 *
 *      }
 *
 *
 *  }
 *
 * // add meta data to properties - using custom registry or a typename class or c++ reflections
 * // create double buffers
 * // buffer a listens to buffer b updates
 * // buffer a is used for game thread
 * // buffer b is used for ui (render) thread
 * // whenever buffer b is updated stack that event
 * // at a sync point in next frame run the event override buffers a data with buffer b
 *
 *  class Component {
 *      int value;
 *
 *  }
 *
 * // how will double buffering act on these????
 * buffer a
 * buffer b
 *
 * a.x = 10; // some how save it as a state
 *
 * */

//


// thread 1 (main)------------- thread 2 (physics)
// dequeue ui input a --------- dequeue device input b
// enqueue device input a ----- enqueue ui input b
// process -------------------- render
// swap sync ------------------ wait

// one way is double buffering for position
// other is input
// int, float, string updated
// int value changed to 3 but of what?
// every field visible on ui should be linked
// --
// ----
// --
// create a buffer for every visible item on editor
// or
// take reference of variable and pass along
// generic event data to copy?
// entity id
// mark buffer changed
// iteration if buffer chagned update for that entity id

// something more dynamic and event based?
// as its only one edit a time


// queue of commands for changes to
// created shared snapshot of component and entity id - with pointer it should automatically revert to original?
// every new data has to be manually taken care for update
// every new data has to be manually shown in the editor to update
// component wise buffer data
// transform buffer and so on...
// rigidbody buffer and so on...

// queues for inputs - half done
// mutex for avoiding race conditions - done
// notify + mutex + locking + conditional for making a thread waitx - done

// let's first look in double buffering
// lets first implement multithreading without double buffering
// then connect the double buffering and multithreading
// then add physics thread with buffering
// then apply changes to each other
// then add dirty flags - check not sure yet
// then add priority checks while apply
// do overall performance checks between each other
// check for change stack with versioning

// for render and main thread
// double buffering
// data sync when items are updated from ui
// a default thread and one new thread for rendering


// create 2 entity - is good idea?
// for(entity in registry)
//  if(position is dirty) or if position.x is dirty , position.y is dirty , position.z is dirty
//  if(rotation is dirty)
//  if(scale is dirty)
// if u think within rigidbody and transform it has it's own state maintaining separate positions

// double bufferring for main thread and renderring
// 2 buffers

// entity registry
// to find specific entity we need id
// entity
// - class component
// -- transform
// ---- position
// ---- rotation
// ---- scale
// ---- applyToRigidbody()
// -- rigidbody
// ---- position
// ---- rotation
// ---- scale
// ---- applyToTransform()
// ---- applyFromTransform()

// entity registry
// to find specific entity we need id

// final thoughts
//here's what I thought for multithreading and syncing between main thread and physics thread
//- main thread creates buffer "a" uses buffer "c" from previous frame
//- merges are applied to buffer "a" using previous frame buffer "b" based priority creating a buffer "c" once main thread and physics thread are done computing and happens after main thread is complete
//- physics thread uses buffer "a" from previous frame and creates buffer "b"
//- render thread uses buffer "a" from previous frame
//and this repeats
//        The new change is, we only wait for main thread to complete. We merge the data of buffer a of current frame and buffer b from previous frame
// e.g.
// a = [1,2,3,...]
// b = [1,2,...]

// for more robust we can think of using job system to further break down threads but for now lets go with what we got lol

// 3 buffers - 1 for swapping, 1 for physics 1 for main thread
// main thread n - 3 - buffer a v1
// physics n - 2 - buffer b v0 on buffer a v0
// rendering n - 1
// merge b v0 on a v1 = buffer c v1
// main thread n - buffer a v2
// physics n + 1 - buffer b v1 on buffer c v1
// merge b v0 and a v1 = buffer c v2
// rendering n + 2


// running physics and main thread parrallely
// both will have a buffer data (double buffering)
// create snapshot of changed data or apply on buffers (snapshot will use extra memory) -not in rendering
// will the changed data take precedence? -not in rendering
// apply anything which doesn't overlap -not in rendering
// apply one to another based on precedence -not in rendering



// using atomic - live updates
// using deltas - difference updates (can lead to accuracy issues)
// using mutex - blocking the thread until work is done (can lead to dependency for waiting for each other)
// method 4 - 2 game states taking precendence upon each other based on changes
// In cases where both the game thread and the physics thread modify the same property (such as position), Unreal Engine typically prioritizes one thread over the other depending on the specific context. For instance:
//
//Physics-Driven Objects: For objects controlled by physics (e.g., rigid bodies), the physics thread's updates take precedence. The game thread reads the final result after physics has finished its simulation step.
//Game-Driven Objects: For objects that the game logic directly controls, the game thread might overwrite the physics simulation results with its own values, depending on the gameplay logic.

// if game thread translates apply that on physics
// translate - physics takes precense over main thread
// set position - take precendence over physics
// we have to create snap shots and check and apply only modified values - how can we do this now?

// interpolation while using data from physics thread to render thread to remove jitters
// if physics thread further ca

// physics uses stuff
// update modified stuff
// render only uses
// start with physics thread
// - need to know delta time (for now we send hard number as delta time)
// - need to have constant time frame
// - need to calculate fixed delta time on its own

// input, fixed update, update, render, ui
// input & update - thread & worker threads
// fixed update - thread
// render scene - thread
// render ui - thread
// frame rate management / main loop - thread?
// frame pipeline - for parallelism ?? ?? ??
// editor ui thread???

// render, physics, main thread
// all will have while loop with can run bool
// stage 1
// start render thread
// start physics thread
// start main thread
// end whne app ends
// stage 2
// add checks if rendering is required
// physics always run on a constant thread time - physics should not depend on frames
// add checks if main thread is required
// with this, we need another thread to tell when frame is over and update is required
// worry about sync later lol - lol become joke worry now

// physics require - transform that also which is dynamic
// game update requires everything to be modified
// render update only uses everything

// if data applied on main thread 1st frame should always override physics thread changes

// 0 1 2 3 4 5
// 0 1 2 3
// 0 - 0 , 1 - 1 , 2 - 2, 3 -3, 4 - 3, 5 - 3
// from 1 to 2  - diff 1
// from 1 to -3  - diff 2

// -3 - 1 + 2 = -2
// 2 - 1 - 3 = -2

// sync 2 to 3 - 1 + 2 = 4
// sync 3 to 2 - 1 + 3 = 4

// new frame data, processed old frame, render processed old frame
// if a change is done , if change is not done
// default x = 1
// on change
// x = 2, x = 1
// apply x = 2 to processed old frame

// if a change is done , if change is done
// default x = 1
// on change
// x = 2, x = 3
// apply x = 2 to processed old frame

// if a change is not done , if change is done
// default x = 1
// on change
// x = 1, x = 3
// apply x = 1 to new frame data

// n     , n - 1 , n - 2
// n + 1 , n     , n - 1
// n + 2 , n + 1 , n

// int, float, bool is hard set, string
// physics modified transform - need to be synced with game thread
// physics can lead to create entities - need to be synced on game thread
// physics deformations can lead to things to be renderered different especially if batching is enabled

// frame 1
// update main thread - buffer a = true = 1
// update physics thread - buffer b = false = 1
// update rendering thread - use buffer c = false = 1

// swap b to c - can be done when physics thread and render thread are done
// swap a to b - can be done when main thread and physics thread are done

// frame 2
// update main thread - buffer c - true = 1 -> 2
// update physics thread - buffer a with updated main thread data - true = 1 -> 3
// update renderring thread - buffer b with updated main physics thread data - false = 1


// frame 3
// update main thread - buffer b with updated main physics thread data true = 2
// update physics thread - buffer c true  3 -> 2
// update renderring thread - buffer a true 1 -> 3

//2 2 2


bool physicat::Application::LoopApplication() {
    PT_PROFILE_SCOPE_N("Main Thread");

    float deltaTime = InternalPointer->TimeStep();
    FpsCounter.frameStart();
    // If Input() returns false - close the application
    if(!Input(deltaTime)) {
        return false;
    }

    if(InternalPointer->FixedStep()) {
        FixedUpdate(InternalPointer->Accumulator + InternalPointer->FixedTimeStep);
    }

    Update(deltaTime);

    // Perform our rendering for this frame, normally you would also perform
    // any updates to your world as well here.
    Render();

    // Returning true means we want to keep looping.
    return true;
}

Application::Application()
    : InternalPointer(physicat::make_internal_ptr<Internal>())
    , IsApplicationRunning(false) {}

//physicat::Application::mainThreadBufferIndex = 0;
FpsCounter physicat::Application::FpsCounter = {};