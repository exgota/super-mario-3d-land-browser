#include <Scene/alScene.h>
#include <math/seadVector.h>
#include <Scene/alSceneObjHolder.h>
#include <Scene/alSceneFunction.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorKit.h>
#include <Layout/alLayoutInitInfo.h>
#include <Layout/alWipeSimple.h>
#include <Nerve/alNerveFunction.h>
#include <Stage/alStageResourceKeeper.h>
#include <File/alFileFunction.h>
#include <Resource/alResource.h>

// Observed ABI views for 0017CB1C. These do not replace the older, incomplete
// CourseSelectScene or ProductStageStartParam declarations. Their source-level
// class names and fields beyond the accessed prefixes remain unrecovered.
namespace CourseSelectSceneReconstruction
{
struct StageSource;
struct StageSourceVTable {
    const char* (*stageName)(StageSource*);
    int (*scenario)(StageSource*);
    int (*world)(StageSource*);
    int (*course)(StageSource*);
};
struct StageSource { const StageSourceVTable* vtable; };
struct CourseIdentifier { int world; int course; };
struct CameraConfig { float values[9]; };
struct CameraConfigPair { CameraConfig main; CameraConfig item; };
struct Camera {
    void* vtable;
    void* director;
    void* camera;
    float position[3];
    u8 unknown18[0x14];
    void* projection;
};
struct Selection { void* vtable; void* selected; int value8; };
struct Actor { u8 prefix[0x68]; int value68; };
struct GameSystem { u8 prefix[0x1c]; void* value1c; };
struct Scene {
    u8 prefix[0x10];
    al::LiveActorKit* actors;
    u8 unknown14[0x10];
    al::StageResourceKeeper* resources;
    u8 unknown28[0x0c];
    StageSource* source;
    void* input38;
    Selection* selection;
    void* player;
    Actor* map;
    void* layout;
    al::NerveStateBase* startState;
    al::NerveStateBase* exitState;
    void* pauseMenu;
    Camera* mainCamera;
    Camera* itemCamera;
    al::WipeSimple* cinemaFrame;
    void* wipe;
    void* resourceState;
};
static_assert(sizeof(Scene) == 0x6c, "Observed scene allocation extent");
static_assert(sizeof(CameraConfig) == 0x24, "Camera constructor configuration");

// Address-point imports refer to existing whole data rows. No table or resource
// bytes are reproduced. The two fixed string layouts are established by the
// stores here and the independent 0027BEC8 constructor.
extern "C" {
extern const u32 dat_003D7AE4[];
extern const u32 dat_003D7ABC[];
extern const u32 dat_003D9C34[];
extern const CameraConfigPair dat_003B16DC;
extern const char dat_003B1728[];
extern const char dat_003B1740[];
extern const char dat_003B1724[];
extern const u32 dat_003F1550;
extern const u32 dat_003F0FA4;
extern const u32 dat_003F0FA0;
extern const u32 dat_003F0FC0;
}
struct StringView { const u32* vtable; const char* text; };
template<int N> struct FixedString {
    const u32* vtable;
    char* text;
    int capacity;
    char buffer[N];
    FixedString(const u32* table) : vtable(table), text(buffer), capacity(N) {
        buffer[N-1] = 0;
        buffer[0] = 0;
    }
    const char* cstr() const {
        typedef void (*AssureTermination)(const void*);
        ((AssureTermination)vtable[2])(this);
        return text;
    }
};
template<int N> struct Allocation { u8 bytes[N]; };

extern "C" {
void* fn_00276858();
void fn_001DE86C(void*);
CourseIdentifier* fn_0027654C(CourseIdentifier*, StageSource*);
int fn_0026AD40(const CourseIdentifier*);
int fn_00216F1C(const CourseIdentifier*);
s32 fn_0028E1E4(sead::BufferedSafeString*, const char*, ...);
sead::Heap* fn_00257F74();
int fn_0026AFBC(StageSource*, int);
bool fn_0025BD08(const CourseIdentifier*);
int fn_002527D8(StageSource*, int, int);
const char* fn_0026AF80(StageSource*, int, int);
void fn_00257F88(const void*, sead::Heap*);
void* fn_00257ED0(void*);
void fn_0016B044();
GameSystem* fn_0027BAAC();
void fn_0016B6C8(void*);
void fn_002767A0(al::Scene*);
float fn_00276774();
Camera* fn_001E6E5C(void*, const char*, const CameraConfig*, float);
void fn_00268FE8(void*, void*, void*);
void fn_00276660(al::Scene*);
al::LayoutInitInfo* fn_0027B31C(al::LayoutInitInfo*);
void fn_00243FF8(al::LayoutInitInfo*, al::LiveActorKit*);
void fn_002765D0(al::ActorInitInfo*, const al::PlacementInfo*, const al::LayoutInitInfo*, al::LiveActorKit*);
void fn_00275748(al::Scene*, const char*, int, const u32*, const StringView*, const char*, int);
void fn_002765A8(void*);
al::ISceneObj* fn_001A69E0(void*, int);
al::ISceneObj* fn_00275638(void*);
void fn_0026EE44();
void fn_002753E8(al::Scene*, const al::Resource*, bool);
void fn_00275054(al::Scene*, const al::Resource*, bool);
Selection* fn_0017BFAC(void*, StageSource*);
void* fn_00188F20(void*, const char*, int, int);
void fn_00188DB0(void*, const al::ActorInitInfo*);
const float* fn_0032BD64(void*);
Actor* fn_0017DA74(void*, StageSource*, void*, const al::ActorInitInfo*, const al::LayoutInitInfo*);
void fn_0017D9A0(Actor*, void*);
void* fn_0018899C(void*, const char*, StageSource*, const al::ActorInitInfo*, const al::LayoutInitInfo*);
void* fn_0013DCCC(void*, const char*, const al::LayoutInitInfo*);
void* fn_0027361C(void*, const char*, const char*, const char*, const al::LayoutInitInfo*, const char*);
void* fn_00277658();
void fn_00274AA8(void*, const al::LayoutInitInfo*);
al::NerveStateBase* fn_001B979C(void*, al::Scene*, StageSource*, void*, const al::ActorInitInfo*, const al::LayoutInitInfo*);
al::NerveStateBase* fn_001B899C(void*, al::Scene*, StageSource*, const al::LayoutInitInfo*);
}

#ifdef NON_MATCHING
extern "C" void fn_0017CB1C(Scene* scene)
{
    al::Scene* base = reinterpret_cast<al::Scene*>(scene);
    fn_001DE86C(fn_00276858());
    FixedString<32> stageName(dat_003D7AE4);
    CourseIdentifier course;
    fn_0027654C(&course, scene->source);
    if (fn_0026AD40(&course))
        fn_0028E1E4(reinterpret_cast<sead::BufferedSafeString*>(&stageName), dat_003B1728, fn_00216F1C(&course));
    else
        fn_0028E1E4(reinterpret_cast<sead::BufferedSafeString*>(&stageName), dat_003B1740, scene->source->vtable->world(scene->source) + 1);
    sead::Heap* resourceHeap = fn_00257F74();
    base->initAndLoadStageResource(stageName.cstr(), 1, resourceHeap);

    int world = scene->source->vtable->world(scene->source);
    int courseCount = fn_0026AFBC(scene->source, world);
    for (int index = 0; index < courseCount; ++index) {
        CourseIdentifier entry = {world, index};
        if (fn_0025BD08(&entry)) {
            FixedString<128> path(dat_003D7ABC);
            int scenario = fn_002527D8(scene->source, world, index);
            const char* name = fn_0026AF80(scene->source, world, index);
            al::makeStageDataArchivePath(reinterpret_cast<sead::BufferedSafeString*>(&path), name, scenario, dat_003B1724);
            fn_00257F88(&path, fn_00257F74());
        }
    }
    void* object = new Allocation<0xc>;
    if (object) object = fn_00257ED0(object);
    scene->resourceState = object;
    fn_0016B044();
    fn_0016B6C8(fn_0027BAAC()->value1c);
    base->initSceneObjHolder();
    base->initActorFactory();
    fn_002767A0(base);

    CameraConfig mainConfig = dat_003B16DC.main;
    mainConfig.values[0] = sead::Vector3f::zero.x;
    mainConfig.values[1] = sead::Vector3f::zero.y;
    mainConfig.values[2] = sead::Vector3f::zero.z;
    object = new Allocation<0x38>;
    Camera* camera = static_cast<Camera*>(object);
    if (object) camera = fn_001E6E5C(object, "上画面３Ｄ", &mainConfig, fn_00276774());
    scene->mainCamera = camera;
    fn_00268FE8(camera->director, camera->camera, camera->projection);
    CameraConfig itemConfig = dat_003B16DC.item;
    itemConfig.values[0] = -474.0f;
    itemConfig.values[1] = -103.0f;
    itemConfig.values[2] = 0.0f;
    object = new Allocation<0x38>;
    camera = static_cast<Camera*>(object);
    if (object) camera = fn_001E6E5C(object, "アイテムストック", &itemConfig, fn_00276774());
    scene->itemCamera = camera;
    fn_00276660(base);

    u32 layoutStorage[3];
    al::LayoutInitInfo& layoutInfo = *reinterpret_cast<al::LayoutInitInfo*>(layoutStorage);
    fn_0027B31C(&layoutInfo);
    fn_00243FF8(&layoutInfo, scene->actors);
    al::PlacementInfo placement;
    al::ActorInitInfo actorInfo;
    fn_002765D0(&actorInfo, &placement, &layoutInfo, scene->actors);
    StringView listener;
    listener.vtable = dat_003D9C34 + 2;
    listener.text = "コースセレクト専用リスナー位置";
    fn_00275748(base, "CourseSelectScene", 0, &dat_003F1550, &listener, "CourseSelectScene", 4);
    fn_002765A8(scene->mainCamera->camera);

    object = new Allocation<8>;
    al::ISceneObj* sceneObject = static_cast<al::ISceneObj*>(object);
    if (object) sceneObject = fn_001A69E0(object, scene->source->vtable->world(scene->source));
    al::setSceneObj(sceneObject, 0x15);
    object = new Allocation<0x3c>;
    sceneObject = static_cast<al::ISceneObj*>(object);
    if (object) sceneObject = fn_00275638(object);
    al::setSceneObj(sceneObject, 8);
    fn_0026EE44();
    fn_002753E8(base, scene->resources->getResourceDesign(), true);
    fn_00275054(base, scene->resources->getResourceDesign(), true);

    object = new Allocation<0x14>;
    Selection* selection = static_cast<Selection*>(object);
    if (object) selection = fn_0017BFAC(object, scene->source);
    scene->selection = selection;
    object = new Allocation<0x40>;
    if (object) object = fn_00188F20(object, "コース選択プレイヤー", scene->source->vtable->course(scene->source), 800);
    scene->player = object;
    fn_00188DB0(object, &actorInfo);
    Camera* positionCamera = scene->mainCamera;
    const float* playerPosition = fn_0032BD64(scene->player);
    positionCamera->position[0] = playerPosition[0];
    positionCamera->position[1] = 190.0f;
    positionCamera->position[2] = 0.0f;
    object = new Allocation<0x8c>;
    Actor* actor = static_cast<Actor*>(object);
    if (object) actor = fn_0017DA74(object, scene->source, scene->selection->selected, &actorInfo, &layoutInfo);
    scene->map = actor;
    al::initCreateActorNoPlacementInfo(reinterpret_cast<al::LiveActor*>(actor), actorInfo);
    scene->map->value68 = scene->selection->value8;
    fn_0017D9A0(scene->map, scene->player);
    object = new Allocation<0x198>;
    if (object) object = fn_0018899C(object, "コースセレクトレイアウト", scene->source, &actorInfo, &layoutInfo);
    scene->layout = object;
    scene->cinemaFrame = new al::WipeSimple("シネマフレーム", "CinemaFrame", layoutInfo, 0);
    object = new Allocation<0x3c>;
    if (object) object = fn_0013DCCC(object, "ポーズメニュー", &layoutInfo);
    scene->pauseMenu = object;
    object = new Allocation<8>;
    if (object) object = fn_0027361C(object, "白フェード", "WipeFadeWhite", "WipeFadeWhiteD", &layoutInfo, 0);
    scene->wipe = object;
    al::initPlacementMap(base, scene->resources->getResourceMap(), actorInfo, "ObjInfo");
    fn_00274AA8(fn_00277658(), &layoutInfo);
    base->endInit(actorInfo);
    base->initNerve(reinterpret_cast<const al::Nerve*>(&dat_003F0FA4), 2);
    object = new Allocation<0x34>;
    al::NerveStateBase* state = static_cast<al::NerveStateBase*>(object);
    if (object) state = fn_001B979C(object, base, scene->source, scene->resourceState, &actorInfo, &layoutInfo);
    scene->startState = state;
    object = new Allocation<0x14>;
    state = static_cast<al::NerveStateBase*>(object);
    if (object) state = fn_001B899C(object, base, scene->source, &layoutInfo);
    scene->exitState = state;
    al::initNerveState(base, scene->startState, reinterpret_cast<const al::Nerve*>(&dat_003F0FA0), "Start");
    al::initNerveState(base, scene->exitState, reinterpret_cast<const al::Nerve*>(&dat_003F0FC0), "Exit");
    scene->actors->endInit();
    world = scene->source->vtable->world(scene->source);
    courseCount = fn_0026AFBC(scene->source, world);
    for (int index = 0; index < courseCount; ++index) {
        CourseIdentifier entry = {world, index};
        if (fn_0025BD08(&entry)) {
            FixedString<128> path(dat_003D7ABC);
            int scenario = fn_002527D8(scene->source, world, index);
            const char* name = fn_0026AF80(scene->source, world, index);
            al::makeStageDataArchivePath(reinterpret_cast<sead::BufferedSafeString*>(&path), name, scenario, dat_003B1724);
            al::findOrCreateResource(*reinterpret_cast<sead::SafeString*>(&path));
        }
    }
}
#endif
} // namespace CourseSelectSceneReconstruction
