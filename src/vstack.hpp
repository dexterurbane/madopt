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
#ifndef MADOPT_VSTACK
#define MADOPT_VSTACK

#include <vector>
#include <map>
#include <string.h>

#include "jacstack.hpp"
#include "hessstack.hpp"

namespace MadOpt {

using namespace std;

class VStack {
    public:
        void doADD(Idx nofelems);

        void doMUL();

        double& lastG();

        void doUnaryOp(const double& jac_value, const double& hess_value);

        void emplace_back(const Idx& id);

        void emplace_back(const double& value);

        void clear();

        Idx size();

        void fill(double* jac, double* hess);

        void optimizeAlignment();

        Idx getNNZ_Jac();

        vector<Idx> getJacEntries();

        vector<HessPair> getHessEntries();

        void setX(const double* xx, const Idx& size);

        Idx data_i=0;

    private:
        vector<double> g_stack;
        Idx g_stack_end=0;
        JacStack jac_stack;
        HessStack hess_stack;
        const double* x;
        Idx x_size=0;

        void ensureElem();
};
}
#endif
