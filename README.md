# Linux userspace DRV2605 driver

## Design Decisions
- named pipes for input
- assume either
  + open loop ERM
  + closed loop LRA

- don't worry about the fancy voltage calculations, `*255/5600` is good enough
