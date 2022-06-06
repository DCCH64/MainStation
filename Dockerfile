FROM balenalib/raspberrypi4-64-node:14-buster-run
RUN install_packages build-essential python3
WORKDIR /usr/src/app
COPY package.json package.json
RUN npm install
COPY . .
CMD npm start