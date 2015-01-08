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
#ifndef MADOPT_INNER_CONSTRAINT_H
#define MADOPT_INNER_CONSTRAINT_H

#include <set>
#include <vector>
#include "common.hpp"

namespace MadOpt {

class Solution;
class ADStack;
class Expr;
typedef char OPType;
class ADStackElem;

class InnerConstraint{
    public:
        InnerConstraint(const Expr& expr, const double _lb, const double _ub, ADStack& stack);

        InnerConstraint(const Expr& expr, ADStack& stack);

        // bounds
        //
        //
        double lb();

        void lb(double v);

        double ub();

        void ub(double v);

        // init
        //
        //
        Idx getNNZ_Jac();

        void getNZ_Jac(unsigned int* jCol);

        void init(HessPosMap& hess_pos_map);

        // compute next point
        //
        //
        void setEvals(const double* x);

        // access next points solution
        //
        //
        const double& getG()const ;

        const vector<double>& getJac()const ;

        void eval_h(double* values, const double& lambda);

        // for access solution
        //
        //
        void setPos(Idx epos);

        Idx getPos()const;

        void setSolutionClass(Solution* sol);

        double lam()const;

        // for debug and testing
        //
        //
        string opsToString()const;

        const string toString() const ;

        const vector<double>& getHess()const ;

        const vector<Idx>& getHessMap()const;

        vector<Idx> getJacEntries();

    private:
        double g;
        vector<double> jac;
        vector<double> hess;

        vector<Idx> hess_map;

        Idx pos=0;
        Solution* sol;

        vector<OPType> operators;
        vector<uintptr_t> data;

        double _lb;
        double _ub;

        ADStack& stack;

        set<Idx> getVarsSet();

        double getX(const double* x, Idx index)const;

        void computeFinalStack(const double* x=nullptr);

        void caseVAR(const double* x, const Idx& pos);

        void caseCONST(const double& value);

        void caseADD(const Idx& counter);

        void caseMUL(const Idx& counter);

        void caseSIN();

        void caseCOS();

        void caseTAN();

        void casePOW(const double& value);

        void doHessJacs(ADStackElem& top, double frst, double scd);

        double getNextValue(Idx& idx); 

        Idx getNextCounter(Idx& idx);

        Idx getNextPos(Idx& idx);

        double getNextParamValue(Idx& idx);
};
}
#endif
