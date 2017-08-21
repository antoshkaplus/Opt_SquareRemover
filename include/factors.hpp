#pragma once



const static vector<double> kLocFactors = {
    0.3, 0.25, 0.25, 0.55, 0.75, 0.8, 1, 0.4, 1
};

const static vector<double> kTripleFactors = {
//    0.00347219, 0.494975, 0.382333, 0.386624, 0.481819, 0.248857, 0.5, 0.926458, 0.332816
    0.382926, 0.158094, 0.223872, 0.995935, 0.146858, 0.149963, 0.995935, 0.707244, 0.64028
//    0.0334822,
//    0.0466378,
//    0.077974,
//    0.0892103,
//    0.0892103,
//    0.235108,
//    0.325278,
//    0.399187,
//    0.0892103
};

const static vector<double> kDoubleFactors = {
    0.35972, 0.471176, 0.348484, 0.382926, 0.494382, 0.2796, 0.382926, 0.627124, 0.841906
};
// for bigger triple population
//const static vector<double> kTripleFactors = {
//0.622099, 0.235108, 0.00406531, 0.300886, 0.608944, 0.995935, 0.218847, 0.412343, 0.648411
//};

// only if you count sq and triples.
const static vector<Count> kBeamWidth = {
    370,
    200,
    80,
    900/5,
    550/3,
    450/7,
    450*3,
    700,
    700
};