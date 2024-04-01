
#include <vector>
#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>

static bool ctest(const std::vector<double>& a, int il, int i, int ir) {
    const double toler = 0.4;

    double s1 = a[i] - a[il];
    double s2 = a[ir] - a[i];
    s1 = s1 * (ir - i);
    s2 = s2 * (i - il);

    return s1 > (s2 + toler);
}

static int right(int n, const std::vector<bool>& h, int i) {
    for (int ir = i + 1; ir <= n; ir++) {
        if (h[ir]) {
            return ir;
        }
    }
    return 0;
}

static int left(const std::vector<bool>& h, int i) {
    for (int il = i - 1; il >= 0; il--) {
        if (h[il]) {
            return il;
        }
    }
    return 0;
}

static void cmerge(int n, const std::vector<double>& a, int i, int m, std::vector<bool>& h) {
    int ir, il, irr, ill;
    bool tstl, tstr;


    // Find the leftmost and rightmost vertices of the upper convex hull
    il = left(h, i);
    ir = right(n, h, i);

    // Check the convexity of the angle formed by IL, I, IR
    if (ctest(a, il, i, ir)) {
        return;
    } else {
        h[i] = false;
        while (true) {
            if (il == (i - m)) {
                tstl = true;
            } else {
                ill = left(h, il);
                tstl = ctest(a, ill, il, ir);
            }
            if (ir == std::min(n, i + m)) {
                tstr = true;
            } else {
                irr = right(n, h, ir);
                tstr = ctest(a, il, ir, irr);
            }
            h[il] = tstl;
            h[ir] = tstr;
            if (tstl && tstr) {
                return;
            }
            if (!tstl) {
                il = ill;
            }
            if (!tstr) {
                ir = irr;
            }
        }
    }
}

static void cnvex(int n, const std::vector<double>& a, std::vector<bool>& h) {
    int i, j, k, m, nj, jc;

    for(i = 0; i < n; i++)
        h[i] = true;

    // Compute K such that N-2 <= 2**K < N-1
    k = static_cast<int>(std::log(n - 2.0) / std::log(2.0));
    if (std::pow(2, k + 1) <= (n - 2))
        k = k + 1;

    // For each M=1,2,4,8,...,2**K
    m = 1;
    for (i = 0; i <= k; i++) {
        nj = std::max(0, static_cast<int>((n - 2 - m) / (m + m)));
        for (j = 0; j <= nj; j++) {
            jc = (j + j + 1) * m + 1;
            cmerge(n, a, jc, m, h);
        }
        m = m + m;
    }
}

static int start(int n, std::vector<double>& a, std::vector<std::complex<double>>& y,
           std::vector<double>& radius, int nz, double small, double big, std::vector<bool>& h) {
    int i, iold, nzeros, j, jj;
    double r, th, ang, temp, xsmall, xbig;
    const double pi2 = 6.2831853071796, sigma = 0.7;

    xsmall = std::log(small);
    xbig = std::log(big);
    nz = 0;
    // Compute the logarithm A(I) of the moduli of the coefficients of the polynomial
    // and then the upper convex hull of the set (A(I),I)
    for (i = 0; i <= n; ++i) {
        if (a[i] != 0.0) {
            a[i] = std::log(a[i]);
        } else {
            a[i] = -1.0e30;
        }
    }
    cnvex(n + 1, a, h);  // Assuming cnvex function is defined

    // Given the upper convex hull of the set (A(I),I), compute the moduli of the starting approximations
    // by means of Rouche's theorem
    iold = 1;
    th = pi2 / n;
    for (i = 1; i <= n + 1; ++i) {
        if (h[i]) {
            nzeros = i - iold;
            temp = (a[iold] - a[i]) / nzeros;

            // Check if the modulus is too small
            if (temp < -xbig && temp >= xsmall) {
                nz += nzeros;
                r = 1.0 / big;
            }
            if (temp < xsmall) {
                nz += nzeros;
            }
            // Check if the modulus is too big
            if (temp > xbig) {
                r = big;
                nz += nzeros;
            }
            if (temp <= xbig && temp > std::max(-xbig, xsmall)) {
                r = std::exp(temp);
            }

            // Compute NZEROS approximations equally distributed in the disk of radius R
            ang = pi2 / nzeros;
            for (j = iold; j < i; ++j) {
                jj = j - iold + 1;
                if (r <= 1.0 / big || r == big) {
                    radius[j] = -1;
                }
                y[j] = r * (std::cos(ang * jj + th * i + sigma) + std::complex<double>(0, 1) * std::sin(ang * jj + th * i + sigma));
            }
            iold = i;
        }
    }

    return nz;
}

static std::complex<double> aberth(int n, int j, const std::vector<std::complex<double>>& root) {
    std::complex<double> abcorr(0.0, 0.0);
    std::complex<double> zj = root[j - 1];
    for (int i = 0; i < j - 1; ++i) {
        std::complex<double> z = zj - root[i];
        abcorr += 1.0 / z;
    }
    for (int i = j; i < n; ++i) {
        std::complex<double> z = zj - root[i];
        abcorr += 1.0 / z;
    }
    return abcorr;
}

static bool newton(int n, const std::vector<std::complex<double>>& poly, const std::vector<double>& apoly,
            const std::vector<double>& apolyr, std::complex<double> z, double small,
            std::complex<double>& corr, double& radius) {
    std::complex<double> p, p1, zi;
    double ap, az, azi, absp;
    bool again;

    az = std::abs(z);
    if (az <= 1) {
        p = poly[n];
        ap = apoly[n];
        p1 = p;
        for (int i = n - 1; i >= 1; --i) {
            p = p * z + poly[i];
            p1 = p1 * z + p;
            ap = ap * az + apoly[i];
        }
        p = p * z + poly[0];
        ap = ap * az + apoly[0];
        corr = p / p1;
        absp = std::abs(p);
        again = (absp > (small + ap));
        if (!again)
            radius = n * (absp + ap) / std::abs(p1);
    } else {
        zi = 1.0 / z;
        azi = 1.0 / az;
        p = poly[n];
        p1 = p;
        ap = apolyr[n];
        for (int i = n - 1; i >= 1; --i) {
            p = p * zi + poly[n - i];
            p1 = p1 * zi + p;
            ap = ap * azi + apolyr[i];
        }
        p = p * zi + poly[0];
        ap = ap * azi + apolyr[0];
        absp = std::abs(p);
        again = (absp > (small + ap));
        std::complex<double> ppsp = (p * z) / p1;
        std::complex<double> den = std::complex<double>(n) * ppsp - 1.0;
        corr = z * (ppsp / den);
        if (again)
            return true;
        radius = std::abs(ppsp) + (ap * az) / std::abs(p1);
        radius = n * radius / std::abs(den);
        radius = radius * az;
    }
    return again;
}

static void polzeros(int n, const std::vector<std::complex<double>>& poly, double eps, double big,
              double small, int nitmax, std::vector<std::complex<double>>& root,
              std::vector<double>& radius, std::vector<bool>& err, int& iter) {
    int i, nzeros;
    std::complex<double> corr, abcorr;
    double amax;
    std::vector<double> apoly(n + 1), apolyr(n + 1);
    const double zero = 0.0;

    // Check consistency of data
    if (std::abs(poly[n]) == zero) {
        throw std::runtime_error("Inconsistent data: the leading coefficient is zero");
    }
    if (std::abs(poly[0]) == zero) {
        throw std::runtime_error("Inconsistent data: the constant term is zero");
    }

    // Compute the moduli of the coefficients
    amax = 0.0;
    for (i = 0; i <= n; ++i) {
        apoly[i] = std::abs(poly[i]);
        amax = std::max(amax, apoly[i]);
        apolyr[i] = apoly[i];
    }

    // Initialize
    for (i = 0; i < n; ++i) {
        radius[i] = zero;
        err[i] = true;
    }

    // Select the starting points
    int nz = 0;
    std::vector<bool> h(n + 1);
    start(n, apolyr, root, radius, nz, small, big, h);

    // Compute the coefficients of the backward-error polynomial
    for (i = 0; i <= n; ++i) {
        apolyr[n - i + 1] = eps * apoly[i] * (3.8 * (n - i) + 1);
        apoly[i] = eps * apoly[i] * (3.8 * i + 1);
    }

    for (i = 0; i < n; ++i) {
        err[i] = true;
        if (radius[i] == -1)
            err[i] = false;
    }

    // Start Aberth's iterations
    for (iter = 1; iter <= nitmax; ++iter) {
        for (i = 0; i < n; ++i) {
            if (err[i]) {
                err[i] = newton(n, poly, apoly, apolyr, root[i], small, corr, radius[i]);
                if (err[i]) {
                    abcorr = aberth(n, i, root);
                    root[i] = root[i] - corr / (1.0 - corr * abcorr);
                } else {
                    nzeros = nz + 1;
                    if (nzeros == n)
                        return;
                }
            }
        }
    }
}

// -------------------------------------------------------------------

using polynomial = std::vector<std::complex<double>>;

// poly is the coefficient of x**(i-1), i=1,...,N+1 of the polynomial
static std::vector<std::complex<double>> roots(polynomial poly) {
    int skipped = 0;
    while (poly.back() == std::complex<double>(0)) {
        poly.pop_back();
        skipped++;
    }
    int N = poly.size() - 1;
    std::vector<std::complex<double>> root(N);
    std::vector<double> radius(N);
    std::vector<bool> err(N);
    int iter;
    double eps = std::numeric_limits<double>::epsilon();
    double big = std::numeric_limits<double>::max();
    double small = std::numeric_limits<double>::min();
    int nitmax = 1000;
    polzeros(N, poly, eps, big, small, nitmax, root, radius, err, iter);
    std::vector<std::complex<double>> root1;
    for (int i = 0; i < N; ++i)
        if (!err[i])
            root1.push_back(root[i]);
    for (int i = 0; i < skipped; ++i)
        root1.push_back(0.0);
    return root1;
}

static double binomial(int n, int k) {
    double res = 1;
    if (k > n - k)
        k = n - k;
    for (int i = 0; i < k; i++) {
        res *= n - i;
        res /= i + 1;
    }
    return res;
}

static polynomial substitutexp1(const polynomial & poly) {
    polynomial poly1(poly.size());
    for (int i = 0; i < poly.size(); ++i) {
        poly1[i] = poly[i];
        for (int j = 0; j < i; ++j)
            poly1[i] += poly[j] * binomial(i, j);
    }
    return poly1;
}

static polynomial deriv(const polynomial & poly) {
    polynomial poly1(poly.size() - 1);
    for (int i = 1; i < poly.size(); ++i)
        poly1[i - 1] = poly[i] * std::complex<double>(i);
    return poly1;
}

static std::complex<double> polevl(const polynomial & poly, const std::complex<double> & x) {
    auto res = poly[0];
    for (int i = 1; i < poly.size(); ++i)
        res = res * x + poly[i];
    return res;
}

static std::complex<double> digamma(std::complex<double> z) {
    auto x = z.real();
    auto y = z.imag();
    auto I = std::complex<double>(0, 1);
    auto r = I * M_PI / 2.0;
    auto oy = y;
    r += std::log(y);
    y *= 2;
    r += I * (1 - 2 * x) / y;
    y *= oy;
    r += (6 * (x - 1) * x + 1) / (6 * y);
    y *= oy;
    r += I * (x - 1) * x * (2 * x - 1) / (3 * y);
    y *= oy;
    auto xsq = x * x;
    auto xcu = xsq * x;
    r += (30 * (xcu * x) - 60 * xcu + 30 * xsq - 1) / (60 * y);
    return r;
}

std::complex<double> sum(const polynomial & p, const polynomial & q) {
    if (p.size() - q.size() >= -1)
        return std::complex<double>(std::numeric_limits<double>::infinity());

    auto p1 = substitutexp1(p);
    auto q1 = substitutexp1(q);
    auto q1d = deriv(q1);
    std::vector<std::complex<double>> r = roots(q1);
    
    for(int i = 0; i < r.size(); ++i) {
        auto root = r[i];
        auto p1root = polevl(p1, root);
        auto q1droot = polevl(q1d, root);
        auto digammaroot = digamma(root);
        auto res = p1root * digammaroot / q1droot;
        r[i] = res;
    }
    // sum all the results
    auto res = r[0];
    for (int i = 1; i < r.size(); ++i)
        res += r[i];
    return res;
}

#include <iostream>
int main() {
    polynomial p = {1.0, 4.0, -2.0};
    polynomial q = {3.0, -4.0, 0, 8.0, -10.0};    
    std::cout << sum(p, q) << std::endl;
}
