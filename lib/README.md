# lib/ — User-space library (Apache 2.0)

> Empty in v0.0.1. The user-space companion library (`libferromem-compress.so`) lands in v0.2.0.

The library will expose a stable C ABI for tools that want to compress / decompress via the FPGA without going through zram (e.g. application-level memory compression, custom storage layers).

See `docs/api.md` §"C ABI for user-space library" for the planned interface.
