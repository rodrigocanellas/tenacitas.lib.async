# tenacitas.lib.async
Header only classes to facilitate asynchronous programming

## Dependending to others `tenacitas.lib.*` 
The corresponding repository must be cloned aside `tenacitas.lib.async`

### Dependencies for the library

`tenacitas.lib.log` (only if `-DTENACITAS_LOG` is used)

`tenacitas.lib.number`

`tenacitas.lib.container`

`tenacitas.lib.calendar`

### Dependencies for the tests

`tenacitas.lib.program`

`tenacitas.lib.test`

`sqlite3` library

### Dependencies for the examples
`Qt`, any version above 5.15

### Building

#### With your build system
The only requirement is that the path to the directory above is in the compiler include path, `-I` in `gcc`.

#### Building tests and examples with QtCreator
The file `tenacitas.lib.async/bld/qtcreator/tenacitas.lib.async.pro` contains the configuration for building tests and examples.



