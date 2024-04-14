# How to run the rapidAPI request with curl
curl --request GET   --url 'https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/modules?ticker=MGB.DE&module=financial-data'    --header 'X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com'      --header 'X-RapidAPI-Key: xx'

# how to load a docker image
docker load < dockerImage.tar