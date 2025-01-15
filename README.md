# Light Logger

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Build Status](https://img.shields.io/github/actions/workflow/status/yourusername/my-awesome-project/ci.yml)
![Version](https://img.shields.io/badge/version-1.0.0-green.svg)

## Description
A light, high performance, lock free file logger.

## Proformance
using std::iostream,  
| t(threads)             | Add to Queue   | Add to File    |
|------------------------|----------------|----------------|
| 24t * 500 logs/t       | 10 ms          | 820 ms         |
| 24t * 5000 logs/t      | 43 ms          | 8363 ms        |
| 24t * 50000 logs/t     | 483 ms         | 74826 ms       |
| 24t * 500000 logs/t    | 2563 ms        | 763619 ms      |

using io_uring,  
| t(threads)             | Add to Queue   | Add to File    |
|------------------------|----------------|----------------|
| 24t * 500 logs/t       | 13 ms          | 38 ms          |
| 24t * 5000 logs/t      | 76 ms          | 343 ms         |
| 24t * 50000 logs/t     | 777 ms         | 5887 ms        |
| 24t * 500000 logs/t    | 2563 ms        | 57039 ms       |

CPU: 12th Gen Intel(R) Core(TM) i9-12900K  
Memory: 16.0GB  
OS: WSL (Ununtu 22.04 on Windows 11)  

## Installation
```bash
git clone https://github.com/Timiamy/ltlog.git
cd ltlog
mkdir build && cd build
cmake ..
make
```

## License
This project is licensed under the MIT License.

## Contact
Email: itimiamy@gmail.com  
GitHub: @Timiamy
