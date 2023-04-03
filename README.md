# tenacitas.lib.async
Header only classes to facilitate asynchronous programming

## Dependending to others `tenacitas.lib.*` 
The corresponding repository must be cloned aside `tenacitas.lib.async`

### Dependencies for the library

`tenacitas.lib.log` (only if `-DTENACITAS_LOG` is used)

`tenacitas.lib.number`

`tenacitas.lib.container`

### Dependencies for the tests

`tenacitas.lib.program`

`tenacitas.lib.test`

`sqlite3` library

### Dependencies for the examples
`Qt`, any version above 5.15

### Building

#### With your build system
The only requirement is that the path to the directory above is in the compiler include path, `-I` in `gcc`.

#### Building the tests with QtCreator
The repository `tenacitas.bld` must be cloned aside `tenacitas.lib.async`. The `.pro` file is in `tenacitas.bld.qtcreator/tst/tenacitas.lib.async` directory.

#### Building the examples with QtCreator
The repository `tenacitas.bld` must be cloned aside `tenacitas.lib.async`. The `.pro` file is in `tenacitas.bld.qtcreator/exp/tenacitas.lib.async` directory.


