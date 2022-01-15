'''
2Aセメスターの授業「プログラミング基礎演習」で作成した独立成分分析(ICA)のコード
重なった画像や音声の分離ができる
本文の説明にならったICA1と外部資料※を参考にしたFastICAの2種類を実装した
※ https://www.slideshare.net/sfchaos/numpy-scipy-9039097
'''


# ICA1
import numpy as np
import scipy as sp
from scipy import linalg

def ICA1(x):
    N,dim = x.shape
    # 平均を0ベクトルに調整
    x = x-x.mean(axis=0)

    # 白色化
    sigma = np.dot(x.T, x)/N
    eigen,E = linalg.eigh(sigma)
    D = np.diag(1/np.sqrt(eigen))
    V = np.dot(np.dot(E,D),E.T)
    z = np.dot(V,x.T)

    # ICA
    # wベクトルの初期化
    w_init = [[1,0],[0,1]]
    s = []
    for w in w_init:
        # 1.wの正規化
        w /= np.linalg.norm(w)
        w_comp = w
        # 2.E[z(w^T z)^3]-3wを計算
        w = np.dot(z,np.dot(w.T,z)**3)/N - 3*w
        # 3.wの正規化
        w /= np.linalg.norm(w)
        # 4.収束判定
        while np.allclose(w,w_comp) == 0:
            w_comp = w
            w = np.dot(z,np.dot(w.T,z)**3)/N - 3*w
            w /= np.linalg.norm(w)
            # -1倍で振動するのを防ぐ
            if w[0] < 0:
                w = -w
        s.append(np.dot(w.T,z))

    return s

# --------------

# FastICA
import numpy as np
import scipy as sp
from scipy import linalg

def fastICA(x, n_comp, alpha=1.0, maxit=200, tol=1e-4):
    n,p = x.shape
    # 中心化
    x = x-x.mean(axis=0)

    # 白色化
    v = np.dot(x.T, x)/n
    u,sigma,v = linalg.svd(v)
    D = np.diag(1/np.sqrt(sigma))
    V = np.dot(D, u.T)
    z = np.dot(V, x.T).T

    # 射影ベクトルの初期化
    w_init = np.random.normal(loc=0, scale=1, size=(n_comp,n_comp))

    # FastICA
    w = ica_deflation(z, n_comp, alpha, maxit, tol, w_init)

    s = np.dot(w, z.T)

    return s


def ica_deflation(x, n_comp, alpha, maxit, tol, w_init):
    n,p = x.shape
    # 求める射影ベクトルの結果を格納するオブジェクト
    w_res = np.zeros((n_comp,n_comp))

    # 各独立成分を求める
    for i in np.arange(n_comp):
        w = w_init[i, ].T
        t = np.zeros((n_comp,np.size(w)))
        # すでに求められている独立成分と直交するように変換
        if i > 0:
            k = np.dot(w_res[0:i, ],w)
            t  = (w_res[0:i, ].T*k).T
        w -= t.sum(axis=0)
        w /= np.linalg.norm(w)
        # 誤差,反復回数カウンタの初期化
        lim  = np.repeat(1000.0,maxit);it = 0
        # 独立成分の探索
        while lim[it] > tol and it < maxit:
            # wpの更新
            wx = np.dot(x,w)
            gwx = np.tanh(alpha*wx)
            gwx = np.repeat(gwx,n_comp).reshape(np.size(gwx),n_comp)
            xgwx = x*gwx
            v1 = xgwx.mean(axis=0)
            g_wx = alpha*(1-(np.tanh(alpha*wx))**2)
            v2 = np.dot(np.mean(g_wx),w)
            w1 = v1-v2
            it += 1
            t = np.zeros((n_comp,np.size(w)))
            # Gram-Schmidtの正規直交化
            if i > 0:
                k = np.dot(w_res[0:i, ],w1)
                t = (w_res[0:i, ].T*k).T
            w1 -= t.sum(axis=0);w1 /= np.linalg.norm(w1)
            # 収束判定
            lim[it] = abs(abs(sum(w1*w))-1.0)
            w = w1
        w_res[i, ] = w
    return w_res
