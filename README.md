
Docker setup:
```
docker build -t gams .
docker run -ti -v $(pwd)/:/home/sim gams /bin/bash
```

Build gams:
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

