#pragma once

//this is for what file your logs want to write
static constexpr auto kLogFilePath = "./ltlog.log";

//this is for the max value of write thread will waiting for
//if there is no log add to ltlog, and the waiting nano second doubles each time from 1 
//and reduced by half each time if pop a log from queue
static constexpr auto kMaxWaitingNanoSecond = 500 * 1000;
