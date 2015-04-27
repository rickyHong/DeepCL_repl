// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#include "ForceBackpropLayerMaker.h"

#include "ForceBackpropLayer.h"

using namespace std;

#undef VIRTUAL
#define VIRTUAL 

ForceBackpropLayer::ForceBackpropLayer( Layer *previousLayer, ForceBackpropLayerMaker *maker ) :
       Layer( previousLayer, maker ),
    outputPlanes( previousLayer->getOutputPlanes() ),
    outputImageSize( previousLayer->getOutputImageSize() ),
    batchSize(0),
    allocatedSize(0),
    output(0) {
}
VIRTUAL ForceBackpropLayer::~ForceBackpropLayer() {
    if( output != 0 ) {
        delete[] output;
    }
}
VIRTUAL std::string ForceBackpropLayer::getClassName() const {
    return "ForceBackpropLayer";
}
VIRTUAL float *ForceBackpropLayer::getOutput() {
    return output;
}
VIRTUAL ActivationFunction const *ForceBackpropLayer::getActivationFunction() {
    return new LinearActivation();
}
VIRTUAL int ForceBackpropLayer::getPersistSize() const {
    return 0;
}
VIRTUAL bool ForceBackpropLayer::needsBackProp() {
    return previousLayer->needsBackProp();
}
VIRTUAL void ForceBackpropLayer::printOutput() const {
    if( output == 0 ) {
         return;
    }
    for( int n = 0; n < std::min(5,batchSize); n++ ) {
        std::cout << "ForceBackpropLayer n " << n << ":" << std::endl;
        for( int plane = 0; plane < std::min( 5, outputPlanes); plane++ ) {
            if( outputPlanes > 1 ) std::cout << "    plane " << plane << ":" << std::endl;
            for( int i = 0; i < std::min(5, outputImageSize); i++ ) {
                std::cout << "      ";
                for( int j = 0; j < std::min(5, outputImageSize); j++ ) {
                    std::cout << getResult( n, plane, i, j ) << " ";
//output[
//                            n * numPlanes * imageSize*imageSize +
//                            plane*imageSize*imageSize +
//                            i * imageSize +
//                            j ] << " ";
                }
                if( outputImageSize > 5 ) std::cout << " ... ";
                std::cout << std::endl;
            }
            if( outputImageSize > 5 ) std::cout << " ... " << std::endl;
        }
        if( outputPlanes > 5 ) std::cout << "   ... other planes ... " << std::endl;
    }
    if( batchSize > 5 ) std::cout << "   ... other n ... " << std::endl;
}
VIRTUAL void ForceBackpropLayer::print() const {
    printOutput();
}
VIRTUAL bool ForceBackpropLayer::needErrorsBackprop() {
    return true; // the main reason for this layer :-)
}
VIRTUAL void ForceBackpropLayer::setBatchSize( int batchSize ) {
    if( batchSize <= allocatedSize ) {
        this->batchSize = batchSize;
        return;
    }
    if( output != 0 ) {
        delete[] output;
    }
    this->batchSize = batchSize;
    this->allocatedSize = allocatedSize;
    output = new float[ getOutputSize() ];
}
VIRTUAL void ForceBackpropLayer::propagate() {
    int totalLinearLength = getOutputSize();
    float *upstreamOutput = previousLayer->getOutput();
    for( int i = 0; i < totalLinearLength; i++ ) {
        output[i] = upstreamOutput[i];
    }
}
VIRTUAL void ForceBackpropLayer::backward( float learningRate, float const *gradOutput ) {
  // do nothing...
}
VIRTUAL int ForceBackpropLayer::getOutputImageSize() const {
    return outputImageSize;
}
VIRTUAL int ForceBackpropLayer::getOutputPlanes() const {
    return outputPlanes;
}
VIRTUAL int ForceBackpropLayer::getOutputCubeSize() const {
    return outputPlanes * outputImageSize * outputImageSize;
}
VIRTUAL int ForceBackpropLayer::getOutputSize() const {
    return batchSize * getOutputCubeSize();
}
VIRTUAL std::string ForceBackpropLayer::toString() {
    return toString();
}
VIRTUAL std::string ForceBackpropLayer::asString() const {
    return std::string("") + "ForceBackpropLayer { outputPlanes=" + ::toString( outputPlanes ) + " outputImageSize=" +  ::toString( outputImageSize ) + " }";
}


