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

#ifndef MADOPT_HESSCSTACK
#define MADOPT_HESSCSTACK

#include "list_cstack.hpp"
#include "common.hpp"

namespace MadOpt {

class JacCStack;

class HessCStack : public ListCStack<PII> {
    public:
        void mergeJacInto(const JacCStack& jac_stack);
        void mergeSingle(const JacCStack& jac_stack,
                const double& jac_value, const double& hess_value);

    private:

};
}
#endif


