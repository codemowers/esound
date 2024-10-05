FROM alpine AS build
RUN apk add alpine-sdk libtool alsa-lib-dev audiofile-dev linux-headers
ADD . .
RUN ./configure --enable-debugging
RUN make install

FROM alpine
RUN apk add audiofile alsa-utils
COPY --from=build /usr/local/ /usr/local/
RUN ldd /usr/local/bin/esd
