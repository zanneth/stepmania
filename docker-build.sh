#!/bin/bash

set -e

if [[ $IN_DOCKER ]]; then
  cd /src/Build
  cmake ..
  make -j$(nproc)
else
  podman build -t zanneth/stepmania-build $(dirname "$0")
  
  # stupid hack since `podman run` can't seem to identify images by name anymore
  image_id=$(podman image ls | grep zanneth/stepmania-build | awk '{print $3}')
  podman run \
    -v $(dirname "$0"):/src \
    -e IN_DOCKER=1 \
    $image_id \
    /src/docker-build.sh

  echo "Successfully built StepMania"
fi

