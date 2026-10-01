#include <Execute/alExecuteDirector.h>
#include <Execute/alExecuteOrder.h>
#include <Execute/alExecuteRequestKeeper.h>
#include <Execute/alExecuteTableHolderDraw.h>
#include <Execute/alExecuteTableHolderUpdate.h>

namespace al
{

ExecuteDirector::ExecuteDirector( int p )
    : _0( p ), mUpdateTable( nullptr ), mDrawTableAmount( 0 ), mDrawTables( nullptr ),
      mRequestKeeper( nullptr ), _14( 0 ), _18( 0 )
{
}

// ARMCC parses CP932 literals bytewise; escape their 0x5C trail bytes.
// Complete retail execution orders: each entry is name, executor type, capacity, category.

static const ExecuteOrder staticd( sUpdateExecuteOrder )[] = {
        { "クリッピング", "Execute", 1, "システム" },
        { "センサー", "Execute", 1, "システム" },
        { "ポインティング", "Execute", 8, "システム" },
        { "空", "ActorMovementCalcAnim", 8, "地形" },
        { "遠景（固定地形）", "ActorMovementCalcAnim", 32, "地形" },
        { "遠景（デモＮＰＣ）", "ActorMovementCalcAnim", 16, "ＮＰＣ" },
        { "遠景", "ActorMovementCalcAnim", 8, "地形" },
        { "固定地形", "ActorMovementCalcAnim", 32, "地形" },
        { "固定地形オブジェ", "ActorMovementCalcAnim", 256, "地形オブジェ" },
        { "固定地形[キャラ後]", "ActorMovementCalcAnim", 32, "地形" },
        { "コリジョン地形", "ActorMovementCalcAnim", 128, "地形" },
        { "コリジョン地形[Movement]", "ActorMovement", 32, "地形" },
        { "コリジョン地形装飾", "ActorMovementCalcAnim", 32, "地形" },
        { "コリジョン地形装飾[Movement]", "ActorMovement", 32, "地形" },
        { "コリジョン地形[キャラ後]", "ActorMovementCalcAnim", 32, "地形" },
        { "コリジョン敵", "ActorMovementCalcAnim", 32, "敵" },
        { "コリジョンアイテム", "ActorMovementCalcAnim", 32, "アイテム" },
        { "コリジョンディレクター", "Execute", 1, "システム" },
        { "中景", "ActorMovementCalcAnim", 8, "地形" },
        { "地形オブジェ", "ActorMovementCalcAnim", 128, "地形オブジェ" },
        { "地形オブジェ[Movement]", "ActorMovement", 32, "地形オブジェ" },
        { "地形オブジェ[キャラ後]", "ActorMovementCalcAnim", 128, "地形オブジェ" },
        { "地形オブジェ[影なし]", "ActorMovementCalcAnim", 64, "地形オブジェ" },
        { "地形オブジェ装飾", "ActorMovementCalcAnim", 32, "地形オブジェ" },
        { "ワープオブジェ[影なし]", "ActorMovementCalcAnim", 32, "地形オブジェ" },
        { "デモ", "ActorMovementCalcAnim", 32, "地形オブジェ" },
        { "デモ[影なし]", "ActorMovementCalcAnim", 32, "地形オブジェ" },
        { "デモ[Movement]", "ActorMovement", 32, "地形オブジェ" },
        { "プレイヤー[Movement]", "ActorMovement", 1, "プレイヤー" },
        { "プレイヤー", "ActorMovementCalcAnim", 8, "プレイヤー" },
        { "プレイヤーモデル", "ActorMovementCalcAnim", 8, "プレイヤー" },
        { "プレイヤー装飾", "ActorMovementCalcAnim", 32, "プレイヤー" },
        { "プレイヤー装飾２", "ActorMovementCalcAnim", 8, "プレイヤー" },
        { "ゴーストプレイヤー記録", "Functor", 1, "敵" },
        { "敵前[Movement]", "ActorMovement", 32, "敵" },
        { "敵", "ActorMovementCalcAnim", 64, "敵" },
        { "敵[Movement]", "ActorMovement", 32, "敵" },
        { "敵装飾", "ActorMovementCalcAnim", 32, "敵" },
        { "ＮＰＣ", "ActorMovementCalcAnim", 32, "ＮＰＣ" },
        { "アイテム", "ActorMovementCalcAnim", 64, "アイテム" },
        { "アイテム装飾", "ActorMovementCalcAnim", 32, "アイテム" },
        { "アイテムストックモデル", "ActorMovementCalcAnim", 32, "アイテム" },
        { "シルエットモデル", "ActorMovementCalcAnim", 16, "影" },
        { "カスタムシルエット", "ActorMovementCalcAnim", 8, "影" },
        { "影[Movement]", "ActorMovement", 8, "影" },
        { "影ボリューム", "ActorMovementCalcAnim", 32, "影" },
        { "プレイヤー影ボリューム", "ActorMovementCalcAnim", 8, "影" },
        { "フォグディレクター", "Execute", 1, "システム" },
        { "ステージスイッチディレクター", "Execute", 1, "システム" },
        { "カメラ振動", "Functor", 1, "システム" },
        { "２Ｄ", "LayoutUpdate", 256, "レイアウト" },
        { "２Ｄ（ポーズ無視）", "LayoutUpdate", 64, "レイアウト" },
        { "エフェクト", "Execute", 1, "エフェクト" },
        { "エフェクト（プレイヤー）", "Execute", 1, "エフェクト" },
        { "エフェクト（ベース２Ｄ）", "Execute", 1, "エフェクト" },
        { "エフェクト（２Ｄ）", "Execute", 1, "エフェクト" },
        { "エフェクト（下画面ベース２Ｄ）", "Execute", 1, "エフェクト" },
        { "エフェクト（下画面２Ｄ）", "Execute", 1, "エフェクト" },
};

static const ExecuteOrder staticd( sDraw0ExecuteOrder )[] = {
        { "空", "ActorModelDraw", 8, "地形" },
        { "遠景（固定地形）", "ActorModelDrawModelCache", 32, "地形" },
        { "遠景", "ActorModelDraw", 8, "地形" },
        { "遠景（デモＮＰＣ）", "ActorModelDraw", 16, "ＮＰＣ" },
};

static const ExecuteOrder staticd( sDraw1ExecuteOrder )[] = {
        { "２Ｄエンドロールテキスト", "LayoutDraw", 64, "レイアウト" },
};

static const ExecuteOrder staticd( sDraw2ExecuteOrder )[] = {
        { "固定地形", "ActorModelDrawModelCache", 32, "地形" },
        { "固定地形オブジェ", "ActorModelDrawModelCache", 256, "地形オブジェ" },
};

static const ExecuteOrder staticd( sDraw3ExecuteOrder )[] = {
        { "コリジョン地形", "ActorModelDraw", 128, "地形" },
        { "コリジョン地形装飾", "ActorModelDraw", 32, "地形" },
        { "コリジョン敵", "ActorModelDraw", 32, "敵" },
        { "コリジョンアイテム", "ActorModelDraw", 32, "アイテム" },
        { "中景", "ActorModelDraw", 8, "地形" },
        { "地形オブジェ", "ActorModelDraw", 128, "地形オブジェ" },
        { "地形オブジェ装飾", "ActorModelDraw", 32, "地形オブジェ" },
        { "カスタムシルエット", "ActorModelDraw", 8, "地形オブジェ" },
        { "デモ", "ActorModelDraw", 32, "地形オブジェ" },
        { "影ボリューム", "ActorModelDraw", 32, "影" },
        { "シルエットモデル", "ActorModelDraw", 16, "影" },
        { "影ボリュームのフィル", "Functor", 1, "影" },
        { "シェーダー[Draw]（影の後）", "Draw", 1, "システム" },
        { "敵", "ActorModelDraw", 128, "敵" },
        { "敵[Draw]", "ActorModelDraw", 8, "敵" },
        { "敵装飾", "ActorModelDraw", 32, "敵" },
        { "ＮＰＣ", "ActorModelDraw", 32, "ＮＰＣ" },
        { "アイテム", "ActorModelDraw", 32, "アイテム" },
        { "アイテム装飾", "ActorModelDraw", 32, "アイテム" },
        { "地形オブジェ[影なし]", "ActorModelDraw", 64, "地形オブジェ" },
        { "ワープオブジェ[影なし]", "ActorModelDraw", 32, "地形オブジェ" },
        { "プレイヤー影ボリューム", "ActorModelDraw", 8, "影" },
        { "プレイヤー影ボリュームのフィル", "Functor", 1, "影" },
        { "シェーダー[Draw]（プレイヤー影の後）", "Draw", 1, "システム" },
        { "プレイヤー", "ActorModelDraw", 8, "プレイヤー" },
        { "プレイヤーモデル", "ActorModelDraw", 8, "プレイヤー" },
        { "プレイヤー装飾", "ActorModelDraw", 32, "プレイヤー" },
        { "プレイヤー装飾２", "ActorModelDraw", 8, "プレイヤー" },
        { "固定地形[キャラ後]", "ActorModelDrawModelCache", 32, "地形" },
        { "コリジョン地形[キャラ後]", "ActorModelDraw", 8, "地形" },
        { "地形オブジェ[キャラ後]", "ActorModelDraw", 128, "地形オブジェ" },
        { "デモ[影なし]", "ActorModelDraw", 32, "地形オブジェ" },
};

static const ExecuteOrder staticd( sDraw4ExecuteOrder )[] = {
        { "２Ｄアイコン", "LayoutDraw", 64, "レイアウト" },
};

static const ExecuteOrder staticd( sDraw5ExecuteOrder )[] = {
        { "２Ｄベース", "LayoutDraw", 64, "レイアウト" },
        { "２Ｄシネマフレーム", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄ", "LayoutDraw", 128, "レイアウト" },
        { "２Ｄ写真", "LayoutDraw", 32, "レイアウト" },
        { "２Ｄヘッド", "LayoutDraw", 32, "レイアウト" },
};

static const ExecuteOrder staticd( sDraw6ExecuteOrder )[] = {
        { "２Ｄポーズ", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄカーソ\ル", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄカーテン", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄカウンター", "LayoutDraw", 16, "レイアウト" },
        { "２Ｄウィンドウ", "LayoutDraw", 16, "レイアウト" },
        { "２Ｄ操作ガイド", "LayoutDraw", 4, "レイアウト" },
        { "２Ｄワイプ", "LayoutDraw", 16, "レイアウト" },
        { "２Ｄカウンター[ワイプ後]", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄゲームオーバー", "LayoutDraw", 8, "レイアウト" },
        { "２Ｄソ\フトリセット", "LayoutDraw", 1, "レイアウト" },
};

static const ExecuteOrder staticd( sDraw7ExecuteOrder )[] = {
        { "２Ｄベース（下画面）", "LayoutDrawBottom", 8, "レイアウト" },
        { "２Ｄ（下画面）", "LayoutDrawBottom", 32, "レイアウト" },
};

static const ExecuteOrder staticd( sDraw8ExecuteOrder )[] = {
        { "アイテムストックモデル", "ActorModelDraw", 32, "アイテム" },
};

static const ExecuteOrder staticd( sDraw9ExecuteOrder )[] = {
        { "２Ｄポーズ（下画面）", "LayoutDrawBottom", 8, "レイアウト" },
        { "２Ｄカーテン（下画面）", "LayoutDrawBottom", 16, "レイアウト" },
        { "２Ｄカウンター（下画面）", "LayoutDrawBottom", 16, "レイアウト" },
        { "２Ｄゲームオーバー背景（下画面）", "LayoutDrawBottom", 8, "レイアウト" },
        { "２Ｄゲームオーバー（下画面）", "LayoutDrawBottom", 8, "レイアウト" },
        { "２Ｄウィンドウ（下画面）", "LayoutDrawBottom", 32, "レイアウト" },
        { "２Ｄカーソ\ル（下画面）", "LayoutDrawBottom", 16, "レイアウト" },
        { "２Ｄワイプ（下画面）", "LayoutDrawBottom", 16, "レイアウト" },
        { "２Ｄソ\フトリセット（下画面）", "LayoutDrawBottom", 1, "レイアウト" },
};

#ifdef NON_MATCHING

void ExecuteDirector::init()
{
        mUpdateTable = new ExecuteTableHolderUpdate;
        mUpdateTable->init( sUpdateExecuteOrder, 58 );
        mDrawTableAmount = 10;
        mDrawTables      = new ExecuteTableHolderDraw*[ mDrawTableAmount ];

        for ( int i = 0; i < mDrawTableAmount; i++ )
        {
                ExecuteTableHolderDraw* newDrawTable = new ExecuteTableHolderDraw;
                mDrawTables[ i ]                     = newDrawTable;
                if ( _14 )
                        newDrawTable->_48 = _14;
                if ( _18 )
                        newDrawTable->_4C = _18;
        }

        mDrawTables[ 0 ]->init( "３Ｄ（空）", sDraw0ExecuteOrder, 4 );
        mDrawTables[ 1 ]->init( "２Ｄエンドロールテキスト（上画面）", sDraw1ExecuteOrder, 1 );
        mDrawTables[ 2 ]->init( "３Ｄ（固定地形）", sDraw2ExecuteOrder, 2 );
        mDrawTables[ 3 ]->init( "３Ｄ（上画面）", sDraw3ExecuteOrder, 32 );
        mDrawTables[ 4 ]->init( "２Ｄアイコン（上画面）", sDraw4ExecuteOrder, 1 );
        mDrawTables[ 5 ]->init( "２Ｄベース（上画面）", sDraw5ExecuteOrder, 5 );
        mDrawTables[ 6 ]->init( "２Ｄ（上画面）", sDraw6ExecuteOrder, 10 );
        mDrawTables[ 7 ]->init( "２Ｄベース（下画面）", sDraw7ExecuteOrder, 2 );
        mDrawTables[ 8 ]->init( "３Ｄ（アイテムストック）", sDraw8ExecuteOrder, 1 );
        mDrawTables[ 9 ]->init( "２Ｄ（下画面）", sDraw9ExecuteOrder, 9 );

        mRequestKeeper = new ExecuteRequestKeeper( _0 );
}

#endif

void ExecuteDirector::createExecutorListTable()
{
        mUpdateTable->createExecutorListTable();
        for ( int i = 0; i < mDrawTableAmount; i++ )
                mDrawTables[ i ]->createExecutorListTable();
}

void ExecuteDirector::registerUser( al::IUseExecutor* p, const char* str )
{
        mUpdateTable->tryRegisterUser( p, str );
        for ( int i = 0; i < mDrawTableAmount; i++ )
                mDrawTables[ i ]->tryRegisterUser( p, str );
}

void ExecuteDirector::registerFunctor( const al::FunctorBase& base, const char* str )
{
        mUpdateTable->tryRegisterFunctor( base, str );
        for ( int i = 0; i < mDrawTableAmount; i++ )
                mDrawTables[ i ]->tryRegisterFunctor( base, str );
}

void ExecuteDirector::registerFunctorDraw( const al::FunctorBase& base, const char* str )
{
        for ( int i = 0; i < mDrawTableAmount; i++ )
                mDrawTables[ i ]->tryRegisterFunctor( base, str );
}

} // namespace al
