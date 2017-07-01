# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Finding parameters

I used twiddle algorithm as it was described in the lectures.

## Effect of PID components

In general the the effect is like expected. But it is more complicated than
it was explained in the lectures, e.g. large P creates oscillations and increasing
 D should help with it, but if D is too large it again creates oscillations. 
 Also I component is almost cannot be detected visually in the simulator.
   
# Result

The final solution is:
```
P = 0.110385
D = 0.0751249
I = 0.000802641
```

For ```1 / deg2rad(25)``` units.
 
The speed is around 50 miles per hour. On a slow computer the car oscillates a bit at such a speed,
so I break when cross track error is to high or when it changes sign frequently which may
indicate oscillation.

# Reflections

* PID has its limitations
    * It doesn't predict future state and acts only based on the past, when it might be
    too late for the action
     * It probably should use different coefficients for different speeds
     * It doesn't consider physics of the car
    
* Lecture material was quite poor. It did not explain what were the limitations
of PID and how to control for them. I think it would be better to put MPC before PID,
 because it makes much easier to understand what is wrong with PID.


---

---


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 