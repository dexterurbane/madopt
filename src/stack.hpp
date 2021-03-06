/*
 * Copyright 2014 National ICT Australia Limited (NICTA)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MADOPT_STACK
#define MADOPT_STACK

#include "common.hpp"

namespace MadOpt {

class Stack {
    public:
        virtual ~Stack(){}

        virtual void doAdd(const Idx& nofelems)=0;
        virtual void doMull()=0; 
        virtual double& lastG()=0;
        virtual void doUnaryOp(const double& jac_value, const double& hess_value)=0;
        virtual void emplace_back(const Idx& id)=0;
        virtual void emplace_back(const double& value)=0;
        virtual Idx size()=0;
        virtual void clear()=0;
        virtual Idx& getDataI()=0;
};
}
#endif

