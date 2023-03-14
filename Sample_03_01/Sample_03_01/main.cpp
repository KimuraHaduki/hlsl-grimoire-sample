#include "stdafx.h"
#include "system/system.h"
#include "TrianglePolygon.h"

//関数宣言
void InitRootSignature(RootSignature& rs);

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    //ゲームの初期化
    InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

    //////////////////////////////////////
    // ここから初期化を行うコードを記述する
    //////////////////////////////////////

    //ルートシグネチャを作成
    RootSignature rootSignature;
    InitRootSignature(rootSignature);

    //三角形ポリゴンを定義
    TrianglePolygon triangle;
    triangle.Init(rootSignature);

    // step-1 定数バッファを作成
    ConstantBuffer cb;
    cb.Init(sizeof(Matrix));

    // step-2 ディスクリプタヒープを作成
    DescriptorHeap ds;
    ds.RegistConstantBuffer(0, cb);
    ds.Commit();

    float posX = 0.5f;
    float posY = 0.4f;
    int number = 0;
    float speed = 1.0f;
    float acceleration = 0.005f;

    const double PI = 3.14159265358979323846; // 円周率
    const int sides = 1440;
    float position[sides][2];
    for (int i = 0; i < sides; i++) {
        double angle = 2 * PI / sides * i; // 角度を計算する
        position[i][0] = cos(angle); // x座標を計算する
        position[i][1] = sin(angle); // y座標を計算する
    }

    //////////////////////////////////////
    // 初期化を行うコードを書くのはここまで！！！
    //////////////////////////////////////
    auto& renderContext = g_graphicsEngine->GetRenderContext();

    // ここからゲームループ
    while (DispatchWindowMessage())
    {
        


        //フレーム開始
        g_engine->BeginFrame();

        //////////////////////////////////////
        //ここから絵を描くコードを記述する
        //////////////////////////////////////

        //ルートシグネチャを設定
        renderContext.SetRootSignature(rootSignature);

        // step-3 ワールド行列を作成
        Matrix mWorld;
        //mWorld.MakeTranslation(0.5f, 0.4f, 0.0f);
        mWorld.MakeTranslation(position[number][0], position[number][1], 0.0f);

        // step-4 ワールド行列をグラフィックメモリにコピー
        cb.CopyToVRAM(mWorld);

        // step-5 ディスクリプタヒープを設定
        renderContext.SetDescriptorHeap(ds);

        //三角形をドロー
        triangle.Draw(renderContext);

        number += (int)speed;
        if (number >= sides) {
            number = 0;
        }
        speed += acceleration;

        /// //////////////////////////////////////
        //絵を描くコードを書くのはここまで！！！
        //////////////////////////////////////
        //フレーム終了
        g_engine->EndFrame();
    }
    return 0;
}

//ルートシグネチャの初期化
void InitRootSignature( RootSignature& rs )
{
    rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
