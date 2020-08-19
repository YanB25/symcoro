# Symmetric Coroutine Worker

A C++ worker implementation based on symmetric coroutine provided by `boost::context`.

It's a very thin wrapper around `boost::context`, relying on the callcc/continuation module.

It's just a couple of lines, so why not read the source code?

## How to Use

``` c++
int main()
{
    symcoro::CoroWorker worker;

    // register a job
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 1" << std::endl;
        return std::move(sink);
    });
    // register yet another job
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 2" << std::endl;
        // Switch away and go back here later
        sink = sink.resume();
        std::cout << "from job 2 again" << std::endl;
        return std::move(sink);
    });
    // register yet another job.
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 3" << std::endl;
        return std::move(sink);
    });

    // block until all the jobs are finished.
    worker.Serve();
}
```

... and the output is 

``` bash
from job 1
from job 2
from job 3
from job 2 again
```

## Prerequisite

- C++ compiler supports at least c++11
- Boost >= 1.4
  - An earlier version of Boost may work, as long as it supports callcc/continuation. But it's not tested.

## Compile

``` bash
mkdir build; cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
# or cmake -DCMAKE_BUILD_TYPE=Debug ..
# for more options, see CMakeLists.txt
make -j8
```

## Run

``` bash
$ ./bin/main
from job 1
from job 2
from job 3
from job 2 again
```

## Install & Uninstall

``` bash
# to install libs and headers
sudo make install
# to uninstall
cat install_manifest.txt | sudo xargs rm -rf
```

## clang-format

`clang-format` is an opt-in if you would like to use.

``` bash
sudo apt install clang-format
```
