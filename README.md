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
| 24t * 500 logs/t       | 6 ms           | 22 ms          |
| 24t * 5000 logs/t      | 31 ms          | 190 ms         |
| 24t * 50000 logs/t     | 267 ms         | 1825 ms        |
| 24t * 500000 logs/t    | 2721 ms        | 18578 ms       |

using io_uring,  
| t(threads)             | Add to Queue   | Add to File    |
|------------------------|----------------|----------------|
| 24t * 500 logs/t       | 6 ms           | 29 ms          |
| 24t * 5000 logs/t      | 29 ms          | 285 ms         |
| 24t * 50000 logs/t     | 281 ms         | 2648 ms        |
| 24t * 500000 logs/t    | 2960 ms        | 49065 ms       |

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
