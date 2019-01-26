
Docker setup:
```
cd simbotic-games
docker build -t gams .
docker run -ti -v $(pwd):/sim --network=host gams /bin/bash
```

Build GAMS:
```
cd $GAMS_ROOT
$GAMS_ROOT/scripts/linux/base_build.sh prereqs clang madara gams
```

Build AirSim:
```
cd $AIRSIM_ROOT
./setup.sh
./build.sh
```
