Clone this repo recursively:
```
git clone --recursive git@github.com:VertexStudio/simbotic-gams.git
```

Docker setup:
```
cd simbotic-games
docker build -t gams .
docker run -ti -v $(pwd):/sim --network=host -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix --cap-add=SYS_PTRACE gams /bin/bash
```

Build GAMS:
```
cd $GAMS_ROOT
$GAMS_ROOT/scripts/linux/base_build.sh prereqs madara gams
```

Build AirSim:
```
cd $AIRSIM_ROOT
./setup.sh
./build.sh
```

Build MultiAgent:
```
cd $SIM_ROOT/multiagent
cmake -G "Unix Makefiles"
make
```
Launch Simbotic and then run `./waypoints.sh`
