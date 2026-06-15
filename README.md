## Command Protocol

For detailed information about the communication protocol, see [COM_PROTOCOL.md](COM_PROTOCOL.md)

### Linux Setup steps

Env;
install conda/miniconda
conda env create -f environment.yml

For Comport access if denied for user;

1. sudo usermod -aG uucp $USER
2. reboot
3. groups
   ls -l /dev/ttyACM0
   should see something like that: crw-rw---- 1 root uucp ...

Download rules for teensy;

```bash
cd /tmp
wget https://www.pjrc.com/teensy/00-teensy.rules
sudo cp /tmp/00-teensy.rules /etc/udev/rules.d/
```
# Note
The project is fully usable and already supports a significant set of features. However, it is still far from complete. Due to a lack of time, active development is currently on hold, and the software should be treated as experimental.
