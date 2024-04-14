curl --request GET   --url 'https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/modules?ticker=MGB.DE&module=financial-data'    --header 'X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com'      --header 'X-RapidAPI-Key: xx'

# How to build and run container
## docker build . -t collector:0.1.0
## docker run --name collector-container -v $(pwd)/bin/config.json:/app/config.json collector:0.1.0