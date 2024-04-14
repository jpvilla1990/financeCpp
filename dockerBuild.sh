releaseDockerImagesPath="release/dockerImages"

versionBackend="0.7.0"
backEndContainer="collector"

versionLlm="0.1.0"
llmContainer="llm"

rm -rf ${releaseDockerImagesPath}/*

docker build . -t ${backEndContainer}:${versionBackend} -f Dockerfile.${backEndContainer}
docker save ${backEndContainer}:${versionBackend} > ${releaseDockerImagesPath}/${backEndContainer}_${versionBackend}.tar

docker build . -t ${llmContainer}:${versionLlm} -f Dockerfile.${llmContainer}
docker save ${llmContainer}:${versionLlm} > ${releaseDockerImagesPath}/${llmContainer}_${versionLlm}.tar