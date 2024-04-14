version="0.7.0"
releaseDockerImagesPath="release/dockerImages"
backEndContainer="collector"

rm -rf ${releaseDockerImagesPath}/*

docker build . -t ${backEndContainer}:${version}
docker save collector:${version} > ${releaseDockerImagesPath}/${backEndContainer}_${version}.tar