releaseDockerImagesPath="release/dockerImages"
registryDockerHub="jpvilla1990"

versionBackend="0.7.0"
backEndContainer="collector"

versionLlm="0.3.0"
llmContainer="llm"

rm -rf ${releaseDockerImagesPath}/*

docker build . -t ${registryDockerHub}/${backEndContainer}:${versionBackend} -f Dockerfile.${backEndContainer}
docker push ${registryDockerHub}/${backEndContainer}:${versionBackend}

docker build . -t ${registryDockerHub}/${llmContainer}:${versionLlm} -f Dockerfile.${llmContainer}
docker push ${registryDockerHub}/${llmContainer}:${versionLlm}