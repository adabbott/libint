
#include <iostream>
#include <math.h>

#include <rr.h>
#include <iter.h>
#include <policy_spec.h>

#include <libint2.h>
#include <test_eri/eri.h>
#include <test_eri/prep_libint2.h>
#include <test_eri_conf.h>

using namespace std;
using namespace libint2;

int main(int argc, char** argv)
{

  typedef unsigned int uint;

  double alpha[4] = {0.5, 1.0, 1.5, 2.0};
  double A[3] = {1.0, 2.0, 3.0};
  double B[3] = {1.5, 2.5, 3.5};
  double C[3] = {4.0, 2.0, 0.0};
  double D[3] = {3.0, 3.0, 1.0};

  SafePtr<CGShell> sh0(new CGShell(&(am[0])));
  SafePtr<CGShell> sh1(new CGShell(&(am[1])));
  SafePtr<CGShell> sh2(new CGShell(&(am[2])));
  SafePtr<CGShell> sh3(new CGShell(&(am[3])));
  
  typedef SubIteratorBase<CGShell> iter;
  SafePtr<iter> sh0_iter(new iter(sh0));
  SafePtr<iter> sh1_iter(new iter(sh1));
  SafePtr<iter> sh2_iter(new iter(sh2));
  SafePtr<iter> sh3_iter(new iter(sh3));

  Libint_t* libint = new Libint_t;
  prep_libint2(libint,am[0],alpha[0],A,
  am[1],alpha[1],B,
  am[2],alpha[2],C,
  am[3],alpha[3],D,0);

  cout << "Testing (" << sh0->label() << sh1->label()
  << "|" << sh2->label() << sh3->label() << ") ..." << endl;

  COMPUTE_XX_ERI_XX(libint);

  bool success = true;
  int ijkl = 0;
  for(sh0_iter->init(); int(*sh0_iter); ++(*sh0_iter)) {
    for(sh1_iter->init(); int(*sh1_iter); ++(*sh1_iter)) {
      for(sh2_iter->init(); int(*sh2_iter); ++(*sh2_iter)) {
        for(sh3_iter->init(); int(*sh3_iter); ++(*sh3_iter), ijkl++) {

          SafePtr<CGF> bf0 = sh0_iter->elem();
          SafePtr<CGF> bf1 = sh1_iter->elem();
          SafePtr<CGF> bf2 = sh2_iter->elem();
          SafePtr<CGF> bf3 = sh3_iter->elem();

          uint l0 = bf0->qn(0);
          uint m0 = bf0->qn(1);
          uint n0 = bf0->qn(2);
          uint l1 = bf1->qn(0);
          uint m1 = bf1->qn(1);
          uint n1 = bf1->qn(2);
          uint l2 = bf2->qn(0);
          uint m2 = bf2->qn(1);
          uint n2 = bf2->qn(2);
          uint l3 = bf3->qn(0);
          uint m3 = bf3->qn(1);
          uint n3 = bf3->qn(2);
  
          double ref_eri = eri(l0,m0,n0,alpha[0],A,
          l1,m1,n1,alpha[1],B,
          l2,m2,n2,alpha[2],C,
          l3,m3,n3,alpha[3],D,0);
  
          double new_eri = libint->targets[0][ijkl];
          
          if ( fabs((ref_eri-new_eri)/new_eri) > 1.0E-12) {
            cout << "Elem ijkl : eri.cc = " << ref_eri
            << " libint = " << new_eri << endl;
            success = false;
          }
        }
      }
    }
  }
  
  cout << "test " << (success ? "ok" : "failed") << endl;
  
}


