//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: distibutionPropsMex.cpp
//
// MATLAB Coder version            : 2.8
// C/C++ source code generated on  : 24-Jul-2015 17:07:45
//

// Include Files
#include "rt_nonfinite.h"
#include "batchEDSolve.h"
#include "distibutionPropsMex.h"
#include "twistlog.h"
#include "batchEDSolve_emxutil.h"
#include "mpower.h"
#include "norm.h"
#include "expm.h"

// Function Definitions

//
// Note: the Matlab bulit-in function logm() is not convertable
//  into a standalone for c/c++ code generation. Another SE3 to twist matrix
//  logarithm is used instead.
//  addpath('/home/roma/Dropbox/2014Summer/Robotics Research/kinematics/kinematics/screws')
//  is needed in the main file that calls this function
// Arguments    : const emxArray_real_T *X
//                double M[16]
//                double Sig[36]
// Return Type  : void
//
void distibutionPropsMex(const emxArray_real_T *X, double M[16], double Sig[36])
{
  double Xsum[16];
  double n;
  int j;
  emxArray_real_T *b_X;
  int b_j1;
  unsigned int j2;
  int i2;
  int i3;
  int i4;
  int loop_ub;
  double M1sum[16];
  double y;
  double b_y[16];
  double C_old;
  double C;
  signed char E[96];
  static const signed char iv9[16] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0,
    0, 0 };

  static const signed char iv10[16] = { 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0 };

  static const signed char iv11[16] = { 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0 };

  static const signed char iv12[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0 };

  static const signed char iv13[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0 };

  static const signed char iv14[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0 };

  int count;
  emxArray_real_T *c_y;
  double M1[16];
  double M2[16];
  double a;
  double M2sum[16];
  double d0;
  int k;
  int k1;
  unsigned int k2;
  int c;
  int cr;
  int ic;
  int br;
  int ar;
  int ib;
  int ia;
  double M1X_logm[16];
  double C1;
  double C2;
  int i;
  int i1;
  unsigned int b_i2;
  int i5;
  int i6;
  double b_Xsum[6];
  double b_M1sum[6];
  memset(&Xsum[0], 0, sizeof(double) << 4);
  n = (double)X->size[1] / 4.0;

  //  Number of matrices
  //  1. Use X to calculate the mean of se(3) Lie algebra
  //  2. Exponentiate the mean of se(3) into Lie group SE(3)
  j = 0;
  emxInit_real_T(&b_X, 2);
  while (j <= (int)n - 1) {
    b_j1 = (j << 2) + 1;
    j2 = (unsigned int)(1 + j) << 2;
    if ((unsigned int)b_j1 > j2) {
      i2 = 0;
      i3 = 0;
    } else {
      i2 = b_j1 - 1;
      i3 = (int)j2;
    }

    i4 = b_X->size[0] * b_X->size[1];
    b_X->size[0] = 4;
    b_X->size[1] = i3 - i2;
    emxEnsureCapacity((emxArray__common *)b_X, i4, (int)sizeof(double));
    loop_ub = i3 - i2;
    for (i3 = 0; i3 < loop_ub; i3++) {
      for (i4 = 0; i4 < 4; i4++) {
        b_X->data[i4 + b_X->size[0] * i3] = X->data[i4 + X->size[0] * (i2 + i3)];
      }
    }

    twistlog(b_X, M1sum);
    for (i2 = 0; i2 < 16; i2++) {
      Xsum[i2] += M1sum[i2];
    }

    j++;
  }

  emxFree_real_T(&b_X);
  y = 1.0 / n;
  for (i2 = 0; i2 < 16; i2++) {
    b_y[i2] = y * Xsum[i2];
  }

  expm(b_y, M);
  C_old = rtInf;
  C = 0.0;

  //  The six Lie algebra elements for SE(3)
  for (i2 = 0; i2 < 4; i2++) {
    for (i3 = 0; i3 < 4; i3++) {
      E[i3 + (i2 << 2)] = iv9[i3 + (i2 << 2)];
      E[16 + (i3 + (i2 << 2))] = iv10[i3 + (i2 << 2)];
      E[32 + (i3 + (i2 << 2))] = iv11[i3 + (i2 << 2)];
      E[48 + (i3 + (i2 << 2))] = iv12[i3 + (i2 << 2)];
      E[64 + (i3 + (i2 << 2))] = iv13[i3 + (i2 << 2)];
      E[80 + (i3 + (i2 << 2))] = iv14[i3 + (i2 << 2)];
    }
  }

  count = 0;
  emxInit_real_T(&c_y, 2);
  while ((fabs(C - C_old) > 0.05) && (count < 10)) {
    C_old = C;
    for (j = 0; j < 6; j++) {
      if (C_old > 20.0) {
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            b_y[i3 + (i2 << 2)] = 0.02 * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M1[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M1[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }

            b_y[i3 + (i2 << 2)] = -0.02 * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M2[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M2[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }
          }
        }
      } else if (C_old > 1.0) {
        a = 0.001 * C_old;
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            b_y[i3 + (i2 << 2)] = a * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        a = -0.001 * C_old;
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M1[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M1[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }

            b_y[i3 + (i2 << 2)] = a * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M2[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M2[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }
          }
        }
      } else {
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            b_y[i3 + (i2 << 2)] = 0.001 * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M1[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M1[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }

            b_y[i3 + (i2 << 2)] = -0.001 * (double)E[(i3 + (i2 << 2)) + (j << 4)];
          }
        }

        expm(b_y, Xsum);
        for (i2 = 0; i2 < 4; i2++) {
          for (i3 = 0; i3 < 4; i3++) {
            M2[i2 + (i3 << 2)] = 0.0;
            for (i4 = 0; i4 < 4; i4++) {
              M2[i2 + (i3 << 2)] += M[i2 + (i4 << 2)] * Xsum[i4 + (i3 << 2)];
            }
          }
        }
      }

      for (i2 = 0; i2 < 16; i2++) {
        M1sum[i2] = 0.0;
        M2sum[i2] = 0.0;
      }

      d0 = (double)X->size[1] / 4.0;
      for (k = 0; k < (int)d0; k++) {
        k1 = (k << 2) + 1;
        k2 = (unsigned int)(1 + k) << 2;
        if ((unsigned int)k1 > k2) {
          i2 = 0;
          i3 = 0;
        } else {
          i2 = k1 - 1;
          i3 = (int)k2;
        }

        mpower(M1, Xsum);
        i4 = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 4;
        emxEnsureCapacity((emxArray__common *)c_y, i4, (int)sizeof(double));
        i4 = c_y->size[0] * c_y->size[1];
        c_y->size[1] = i3 - i2;
        emxEnsureCapacity((emxArray__common *)c_y, i4, (int)sizeof(double));
        loop_ub = (i3 - i2) << 2;
        for (i4 = 0; i4 < loop_ub; i4++) {
          c_y->data[i4] = 0.0;
        }

        if (i3 - i2 == 0) {
        } else {
          c = ((i3 - i2) - 1) << 2;
          for (cr = 0; cr <= c; cr += 4) {
            for (ic = cr; ic + 1 <= cr + 4; ic++) {
              c_y->data[ic] = 0.0;
            }
          }

          br = 0;
          for (cr = 0; cr <= c; cr += 4) {
            ar = 0;
            for (ib = br; ib + 1 <= br + 4; ib++) {
              if (X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))] != 0.0) {
                ia = ar;
                for (ic = cr; ic + 1 <= cr + 4; ic++) {
                  ia++;
                  c_y->data[ic] += X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))]
                    * Xsum[ia - 1];
                }
              }

              ar += 4;
            }

            br += 4;
          }
        }

        twistlog(c_y, M1X_logm);
        if (k1 > (int)k2) {
          i2 = 0;
          i3 = 0;
        } else {
          i2 = k1 - 1;
          i3 = (int)k2;
        }

        mpower(M2, Xsum);
        i4 = c_y->size[0] * c_y->size[1];
        c_y->size[0] = 4;
        emxEnsureCapacity((emxArray__common *)c_y, i4, (int)sizeof(double));
        i4 = c_y->size[0] * c_y->size[1];
        c_y->size[1] = i3 - i2;
        emxEnsureCapacity((emxArray__common *)c_y, i4, (int)sizeof(double));
        loop_ub = (i3 - i2) << 2;
        for (i4 = 0; i4 < loop_ub; i4++) {
          c_y->data[i4] = 0.0;
        }

        if (i3 - i2 == 0) {
        } else {
          c = ((i3 - i2) - 1) << 2;
          for (cr = 0; cr <= c; cr += 4) {
            for (ic = cr; ic + 1 <= cr + 4; ic++) {
              c_y->data[ic] = 0.0;
            }
          }

          br = 0;
          for (cr = 0; cr <= c; cr += 4) {
            ar = 0;
            for (ib = br; ib + 1 <= br + 4; ib++) {
              if (X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))] != 0.0) {
                ia = ar;
                for (ic = cr; ic + 1 <= cr + 4; ic++) {
                  ia++;
                  c_y->data[ic] += X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))]
                    * Xsum[ia - 1];
                }
              }

              ar += 4;
            }

            br += 4;
          }
        }

        twistlog(c_y, Xsum);
        for (i2 = 0; i2 < 16; i2++) {
          M1sum[i2] += M1X_logm[i2];
          M2sum[i2] += Xsum[i2];
        }
      }

      a = b_norm(M1sum);
      C1 = a * a;
      a = b_norm(M2sum);
      C2 = a * a;
      if (C1 <= C2) {
        C = C1;
        memcpy(&M[0], &M1[0], sizeof(double) << 4);
      } else {
        C = C2;
        memcpy(&M[0], &M2[0], sizeof(double) << 4);
      }
    }

    count++;
  }

  memset(&Sig[0], 0, 36U * sizeof(double));
  d0 = (double)X->size[1] / 4.0;
  for (i = 0; i < (int)d0; i++) {
    i1 = (i << 2) + 1;
    b_i2 = (unsigned int)(1 + i) << 2;
    if ((unsigned int)i1 > b_i2) {
      i2 = 0;
      i3 = 0;
    } else {
      i2 = i1 - 1;
      i3 = (int)b_i2;
    }

    if (i1 > (int)b_i2) {
      i4 = 0;
      i5 = 0;
    } else {
      i4 = i1 - 1;
      i5 = (int)b_i2;
    }

    mpower(M, Xsum);
    i6 = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 4;
    emxEnsureCapacity((emxArray__common *)c_y, i6, (int)sizeof(double));
    i6 = c_y->size[0] * c_y->size[1];
    c_y->size[1] = i3 - i2;
    emxEnsureCapacity((emxArray__common *)c_y, i6, (int)sizeof(double));
    loop_ub = (i3 - i2) << 2;
    for (i6 = 0; i6 < loop_ub; i6++) {
      c_y->data[i6] = 0.0;
    }

    if (i3 - i2 == 0) {
    } else {
      c = ((i3 - i2) - 1) << 2;
      for (cr = 0; cr <= c; cr += 4) {
        for (ic = cr; ic + 1 <= cr + 4; ic++) {
          c_y->data[ic] = 0.0;
        }
      }

      br = 0;
      for (cr = 0; cr <= c; cr += 4) {
        ar = 0;
        for (ib = br; ib + 1 <= br + 4; ib++) {
          if (X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))] != 0.0) {
            ia = ar;
            for (ic = cr; ic + 1 <= cr + 4; ic++) {
              ia++;
              c_y->data[ic] += X->data[ib % 4 + X->size[0] * (i2 + (ib >> 2))] *
                Xsum[ia - 1];
            }
          }

          ar += 4;
        }

        br += 4;
      }
    }

    twistlog(c_y, M1sum);

    // Function to vectorize or hat an element of se(3)
    // files needed:
    // none
    // If input is skew-sym change to vector
    mpower(M, Xsum);
    i2 = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 4;
    emxEnsureCapacity((emxArray__common *)c_y, i2, (int)sizeof(double));
    i2 = c_y->size[0] * c_y->size[1];
    c_y->size[1] = i5 - i4;
    emxEnsureCapacity((emxArray__common *)c_y, i2, (int)sizeof(double));
    loop_ub = (i5 - i4) << 2;
    for (i2 = 0; i2 < loop_ub; i2++) {
      c_y->data[i2] = 0.0;
    }

    if (i5 - i4 == 0) {
    } else {
      c = ((i5 - i4) - 1) << 2;
      for (cr = 0; cr <= c; cr += 4) {
        for (ic = cr; ic + 1 <= cr + 4; ic++) {
          c_y->data[ic] = 0.0;
        }
      }

      br = 0;
      for (cr = 0; cr <= c; cr += 4) {
        ar = 0;
        for (ib = br; ib + 1 <= br + 4; ib++) {
          if (X->data[ib % 4 + X->size[0] * (i4 + (ib >> 2))] != 0.0) {
            ia = ar;
            for (ic = cr; ic + 1 <= cr + 4; ic++) {
              ia++;
              c_y->data[ic] += X->data[ib % 4 + X->size[0] * (i4 + (ib >> 2))] *
                Xsum[ia - 1];
            }
          }

          ar += 4;
        }

        br += 4;
      }
    }

    twistlog(c_y, Xsum);

    // Function to vectorize or hat an element of se(3)
    // files needed:
    // none
    // If input is skew-sym change to vector
    b_Xsum[0] = -Xsum[9];
    b_Xsum[1] = Xsum[8];
    b_Xsum[2] = -Xsum[4];
    b_Xsum[3] = Xsum[12];
    b_Xsum[4] = Xsum[13];
    b_Xsum[5] = Xsum[14];
    b_M1sum[0] = -M1sum[9];
    b_M1sum[1] = M1sum[8];
    b_M1sum[2] = -M1sum[4];
    b_M1sum[3] = M1sum[12];
    b_M1sum[4] = M1sum[13];
    b_M1sum[5] = M1sum[14];
    for (i2 = 0; i2 < 6; i2++) {
      for (i3 = 0; i3 < 6; i3++) {
        Sig[i2 + 6 * i3] += b_M1sum[i2] * b_Xsum[i3];
      }
    }
  }

  emxFree_real_T(&c_y);
  y = 1.0 / (double)X->size[1] / 4.0;
  for (i2 = 0; i2 < 36; i2++) {
    Sig[i2] *= y;
  }
}

//
// File trailer for distibutionPropsMex.cpp
//
// [EOF]
//
