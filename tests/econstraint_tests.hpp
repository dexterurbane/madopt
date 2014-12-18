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
#include <cxxtest/TestSuite.h>
#include "testmodel.hpp"
#include "../src/econstraint.hpp"
#include "tutorial_obj_term.hpp"
#include "tutorial_constraint.hpp"

using namespace MadOpt;

class EConstraintTest: public CxxTest::TestSuite {
    public:

        void Tes(Expr exp, 
                const vector<double> x,
                const double g,
                const vector<Idx> jac_entries, 
                const vector<double> jac,
                const vector<PII> hess_entries={}, 
                const vector<double> hess={}, 
                const double delta=0.000001
                ){
            ADStack stack;
            EConstraint e(exp, stack);
            HessPosMap hess_pos_map;
            e.init(hess_pos_map);

            map<int, double> jacvm;
            for (Idx i=0; i<jac_entries.size(); i++)
                jacvm[jac_entries[i]] = jac[i];

            map<PII, double> hessvm;
            for (Idx i=0; i<hess_entries.size(); i++)
                hessvm[hess_entries[i]] = hess[i];

            auto ejace = e.getJacEntries();

            e.setEvals(x.data());

            map<int, double> ej;
            auto ejac = e.getJac();

            for (Idx i=0; i<e.getNNZ_Jac(); i++)
                ej[ejace[i]] = ejac[i];

            vector<double> ehess(hess_pos_map.size(), 0);
            auto hess_map = e.getHessMap();
            auto hess_res = e.getHess();
            int i=0;
            for (auto x: hess_map)
                ehess[x] += hess_res[i++];

            map<PII, double> eh;
            for (auto p: hess_pos_map)
                eh[p.first] = ehess[p.second];

            TS_ASSERT_DELTA(e.getG(), g, delta);
            TS_ASSERT_EQUALS(ej.size(), jacvm.size());
            TS_ASSERT_EQUALS(eh.size(), hessvm.size());

            if (delta == 0){
                TS_ASSERT_EQUALS(ej, jacvm);
                TS_ASSERT_EQUALS(eh, hessvm);
            } else {

                for (auto p: ej)
                    TS_ASSERT_DELTA(p.second, jacvm.at(p.first), delta);

                for (auto p: eh)
                    TS_ASSERT_DELTA(p.second, hessvm.at(p.first), delta);
            }
        }

        void testVar(){
            TestModel m;
            Var a = m.addVar("a");
            Tes(a, {3}, 3, {0}, {1});
        }

        void testParam(){
            TestModel m;
            Var a = m.addVar("a");
            Param x = m.addParam(3, "x");
            Tes(x*a, {2}, 6, {0}, {3});
            x.value(4);
            Tes(x*a, {2}, 8, {0}, {4});
        }

        void testADD(){
            TestModel m;
            Var a = m.addVar("a");
            Var b = m.addVar("b");
            Tes(a+b, {2, 3}, 2+3, {0, 1}, {1,1});
        }

        void testMUL(){
            TestModel m;
            Var a = m.addVar("a");
            Var b = m.addVar("b");
            Tes(2*a, {3}, 6, {0}, {2});
            Tes(a*b, {2,3}, 2*3, {0,1}, {3,2}, {PII(0,1)}, {1});
            double ax = 3;
            double bx = 5;
            Tes(a*a*b, {ax,bx}, 
                ax*ax*bx, 
                {0,1}, {2*ax*bx, ax*ax},
                {PII(0,0), PII(0,1)}, {2*bx, 2*ax});
        }

        void testCOS(){
            TestModel m;
            Var a = m.addVar("a");
            Tes(cos(2*a), {3}, cos(2*3), {0}, {-2*sin(6)}, {PII(0,0)}, {-4*cos(6)});
        }

        void testPOW(){
            TestModel m;
            Var a = m.addVar("a");
            Tes(pow(a,2), {3}, pow(3,2), {0}, {2*3}, {PII(0,0)}, {2});
            Tes(pow(2*a, 3), {3}, pow(6,3), {0}, {3*2*pow(2*3,2)}, {PII(0,0)}, {3*2*2*2*pow(2*3,1)});
        }

        void testMulti(){
            TestModel m;
            Var a = m.addVar("a");
            Var b = m.addVar("b");
            Var c = m.addVar("c");
            Tes(2*a+b, {2, 3}, 2*2+3, {0,1}, {2,1});
            Tes(c*a+b, {2,3,4}, 4*2+3, {0,1,2}, {4,1,2}, {PII(0,2)}, {1});
            double ax = 3;
            double bx = 2;
            Tes((2*a+1.5)*b, {ax, bx}, (2*ax+1.5)*bx,
                {0,1}, {2*bx, 2*ax+1.5}, {PII(0,1)}, {2});
        }

        void testBug(){
            TestModel m;
            Var a = m.addVar("a");
            Var b = m.addVar("b");

            double ax = 3;
            double bx = 2;
            double v = 3.0/100.0;

            Tes((a*a + 1.5*a - v)*b, {ax,bx}, 
                (ax*ax + 1.5*ax-v)*bx, 
                {0,1}, {(2*ax+1.5)*bx, ax*ax+1.5*ax-v},
                {PII(0,0), PII(0,1)}, {2*bx, 2*ax+1.5});
        }

        void testTutorialTerm(){
            TestModel m;
            Var a = m.addVar("a");
            Var b = m.addVar("b");
            Var c = m.addVar("c");

            double ax = 3.1;
            double bx = 2;
            double cx = 1;
            double v = 3.0/100;

            Tes((pow(a,2) + 1.5*a - v)*cos(b)-c, {ax,bx,cx}, 
                (pow(ax,2)+1.5*ax-v)*cos(bx)-cx, 
                {0,1,2}, {(2*ax+1.5)*cos(bx), -(pow(ax,2)+1.5*ax-v)*sin(bx), -1},
                {PII(0,0), PII(0,1), PII(1,1)},
                {cos(bx)*2, -sin(bx)*(2*ax+1.5), -(pow(ax,2)+1.5*ax-v)*cos(bx)});
        }

        void testTutorial(){
            int N = 100;
            int nah = N-1 + N-2;
            TestModel m;
            vector<Var> x(N);
            vector<double> x_buffer(N);
            for (int i=0; i<N; i++){
                x[i] = m.addVar(-1.5, 0, -0.5, "x" + to_string(i));
                x_buffer[i] = i;
            }
            HessPosMap hess_pos_map;
            for (int i=0; i<N-2; i++){
                double a = double(i+2)/(double)N;
                TutorialConstraint t(i, a);
                t.init(hess_pos_map);
                t.setEvals(x_buffer.data());
                Tes((pow(x[i+1], 2) + 1.5*x[i+1] - a)*cos(x[i+2]) - x[i],
                        x_buffer, t.getG(), 
                        t.getJacEntries(), t.getJac(),
                        t.getHessEntries(), t.getHess(), 0.00001);
            }
        }

        void testSumObj(){
            int N = 100;
            TestModel m;
            vector<Var> x(N);
            Expr obj(0);
            vector<double> x_data(x.size());
            for (int i=0; i<N; i++){
                x[i] = m.addVar(-1.5, 0, -0.5, "x" + to_string(i));
                obj += pow(x[i] - 1, 2);
                x_data[i] = i;
            }
            TutorialObjTerm t(N, 0);
            HessPosMap hess_pos_map;
            t.init(hess_pos_map);
            t.setEvals(x_data.data());

            Tes(obj, x_data, t.getG(), t.getJacEntries(), t.getJac(), 
                    t.getHessEntries(), vector<double>(N,2), 0);
        }

        void testCaseADD(){
            for (int N=1; N<13; N++){
                TestModel m;
                Expr term(0);
                vector<double> x(N);
                vector<Idx> jac_entries(N);
                vector<double> jac(N);
                vector<PII> hess_entries(N);
                vector<double> hess(N);
                double g = 0;
                for (int i=0; i < N; ++i) {
                    term += pow(m.addVar("x"), 2);
                    x[i] = i;
                    g += i*i;
                    jac_entries[i] = i;
                    jac[i] = 2*i;
                    hess_entries[i] = PII(i,i);
                    hess[i] = 2;
                }

                Tes(term, x, g, jac_entries, jac, hess_entries, hess);
            }
        }

        void testLargeSum(){
            int N = 4;
            TestModel m;
            Expr term(0);
            vector<double> x(N);
            vector<Idx> jac_entries(N);
            vector<double> jac(N);
            vector<PII> hess_entries(N);
            vector<double> hess(N);
            double g = 0;
            for (int i=0; i < N; ++i) {
                term += pow(m.addVar("x" + std::to_string(i)), 2);
                x[i] = i;
                g += i*i;
                jac_entries[i] = i;
                jac[i] = 2*i;
                hess_entries[i] = PII(i,i);
                hess[i] = 2;
            }

            Tes(term, x, g, jac_entries, jac, hess_entries, hess);
        }

        void testLargeSum2(){
            int N = 4;
            TestModel m;
            vector<Var> vars(N);
            vector<double> x(N);
            vector<Idx> jac_entries(N);
            vector<double> jac(N);
            vector<PII> hess_entries(2*(N-1)+1);
            vector<double> hess(2*(N-1)+1);

            for (int i=0; i < N; ++i) {
                vars[i] = m.addVar("x");
                x[i] = i;
                jac_entries[i] = i;
            }

            Expr term(0);
            double g = 0;
            Idx hess_counter = 0;
            for (int i=0; i < N-1; ++i) {
                term += pow(vars[i] + vars[i+1], 2);
                g += (2*i+1)*(2*i+1);
            }

            hess_entries[hess_counter] = PII(0,0);
            hess[hess_counter++] = 2;
            hess_entries[hess_counter] = PII(0,1);
            hess[hess_counter++] = 2;
            jac[0] = 2*(0 + 1);
            for (int i=1; i < N-1; ++i) {
                jac[i] = 2*(i+i+1) + 2*(i-1+i);
                hess_entries[hess_counter] = PII(i,i);
                hess[hess_counter++] = 2+2;
                hess_entries[hess_counter] = PII(i,i+1);
                hess[hess_counter++] = 2;
            }
            jac[N-1] = 2*(N-2 + N-1);
            hess_entries[hess_counter] = PII(N-1,N-1);
            hess[hess_counter++] = 2;

            Tes(term, x, g, jac_entries, jac, hess_entries, hess);
        }
};
