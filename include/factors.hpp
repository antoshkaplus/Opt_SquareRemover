#pragma once



const static vector<double> kLocFactors = {
    0.26955, 0.168144, 0.154988, 0.940207, 0.741686, 0.943312, 0.566371, 0.853142, 0.471176
};

const static vector<double> kTripleFactors = {
    0.00347219, 0.494975, 0.382333, 0.386624, 0.481819, 0.248857, 0.5, 0.926458, 0.332816

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
    3*450/4,
    3*225/4,
    3*100/4,
    900/5,
    550/3,
    450/7,
    450*3,
    225*2.8,
    225*2.95
};