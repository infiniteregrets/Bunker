FROM gcc:latest 
RUN apt-get update -y && apt-get install -y vim 
WORKDIR /Bunker 
COPY ./ ./ 