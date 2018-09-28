FROM gcc:5.4
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make filter
CMD make run