FROM 431983620723.dkr.ecr.ap-east-1.amazonaws.com/dm/x-r-run-dev:0e5ac2bf8800754d38b56231f750f6b8f3e09774

RUN apt-get update && apt install -y --no-install-recommends \
    openjdk-17-jdk openjdk-17-jre \
    && rm -rf /var/lib/apt/lists/*
