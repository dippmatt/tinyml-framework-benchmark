# Use an official Ubuntu image as a base
FROM ubuntu:20.04

# Set environment variable to suppress interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Copy the eIQ_Glow_Linux64.deb.bin installer into the container
COPY eIQ_Glow_Linux64.deb.bin /opt/

RUN apt-get update && apt-get install -y whiptail libprotobuf-dev libpng16-16 libgoogle-glog-dev	

# Make the installer executable
RUN chmod +x /opt/eIQ_Glow_Linux64.deb.bin

# Set the working directory to where Glow binaries are installed
WORKDIR /opt

# Add Glow binaries to the PATH
ENV PATH="/opt/nxp/Glow/bin:${PATH}"

# Default command to keep the container alive
CMD ["/bin/bash"]
