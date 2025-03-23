CONTAINER_NAME=$1
IMAGE_NAME="jamesnulliu/grafixboard/v0.1:vtune2025.0.2-ubuntu24.04"

if [ ! $CONTAINER_NAME ]; then
    CONTAINER_NAME=tmp
fi

docker run -td \
    --pid=host \
    --cap-add=SYS_ADMIN \
    --cap-add=SYS_PTRACE \
    --name $CONTAINER_NAME \
    --gpus all \
    --network=host \
    --entrypoint /bin/bash \
    -v $HOME/Projects:/root/Projects \
    $IMAGE_NAME


docker cp $HOME/.ssh $CONTAINER_NAME:/root/
docker start $CONTAINER_NAME
docker exec $CONTAINER_NAME chown -R root:root /root/.ssh
