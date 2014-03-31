TOP_PATH := $(call my-dir)
LOCAL_PATH := $(TOP_PATH)
EXTERN_PATH := $(TOP_PATH)/../extern
ANDEXTERN_PATH := $(EXTERN_PATH)/android

include $(CLEAR_VARS)

$(TOP_PATH)/ver.cpp: ; \
 if test -e $(TOP_PATH)/ver.cpp; then \
   build=`sed -rs 's/.*version_num = ([[:digit:]]+);/\1/;q' $$(TOP_PATH)/ver.cpp`; \
   build=`expr $$build + 1`; \
 else \
   build=0; \
 fi; \
 echo "extern const unsigned long version_num = $$build;" > $(TOP_PATH)/ver.cpp; \
 echo "extern const char *const version_time = \"`date +"%X %Z (UTC%:z)"`\";" >> $(TOP_PATH)/ver.cpp; \
 echo "extern const char *const version_date = \"`date +"%Y%m%d"`\";" >> $(TOP_PATH)/ver.cpp;

LOCAL_MODULE:= stepmania

# Architecture-specific Sources
PLATFORM_SRC   := arch/InputHandler/InputHandler_Android_Gamepad.cpp \
                  arch/LoadingWindow/LoadingWindow_Android.cpp \
                  arch/LowLevelWindow/EGLProviders/EGLProvider_Android.cpp \
                  arch/LowLevelWindow/EGLProviders/EGLProvider.cpp \
                  arch/LowLevelWindow/LowLevelWindow_EGL.cpp \
                  arch/ArchHooks/ArchHooks_Android.cpp \
                  archutils/Android/CrashHandler.cpp \
                  archutils/Android/Globals.cpp \
                  archutils/Android/SpecialDirs.cpp \
                  arch/Sound/RageSoundDriver_Android.cpp

                  #archutils/Android/Framework/cpp/stepstart.cpp \
# Shared, Non-Core Sources
SHARED_ARCH_SRC:= arch/RageDriver.cpp \
                  arch/LoadingWindow/LoadingWindow.cpp \
                  arch/Lights/LightsDriver.cpp \
                  arch/Lights/LightsDriver_Export.cpp \
                  arch/InputHandler/InputHandler.cpp \
                  arch/InputHandler/InputHandler_MonkeyKeyboard.cpp \
                  arch/LowLevelWindow/LowLevelWindow.cpp \
                  arch/Dialog/Dialog.cpp \
                  arch/Dialog/DialogDriver.cpp \
                  arch/ArchHooks/ArchHooks.cpp \
                  arch/ArchHooks/ArchHooksUtil.cpp \
                  arch/MemoryCard/MemoryCardDriver.cpp \
                  arch/MovieTexture/MovieTexture_Null.cpp \
                  arch/MovieTexture/MovieTexture.cpp \
                  arch/MovieTexture/MovieTexture_Generic.cpp \
                  arch/MovieTexture/MovieTexture_FFMpeg.cpp \
                  arch/Threads/Threads_Pthreads.cpp \
                  archutils/Common/PthreadHelpers.cpp \
                  archutils/Common/EGLHelper.cpp \
                  archutils/Unix/GetSysInfo.cpp \
                  archutils/Unix/StackCheck.cpp \
                  archutils/Unix/BacktraceNames.cpp \
                  archutils/Unix/EmergencyShutdown.cpp \
                  arch/Sound/RageSoundDriver.cpp \
                  arch/Sound/RageSoundDriver_Generic_Software.cpp

# The Rage engine itself
RAGE_ENGINE    := RageBitmapTexture.cpp \
                  RageDisplay.cpp \
                  RageDisplay_GLES2.cpp \
                  RageDisplay_Null.cpp \
                  RageException.cpp \
                  RageFile.cpp \
                  RageFileBasic.cpp \
                  RageFileDriver.cpp \
                  RageFileDriverDeflate.cpp \
                  RageFileDriverDirect.cpp \
                  RageFileDriverDirectHelpers.cpp \
                  RageFileDriverMemory.cpp \
                  RageFileDriverReadAhead.cpp \
                  RageFileDriverSlice.cpp \
                  RageFileDriverTimeout.cpp \
                  RageFileDriverZip.cpp \
                  RageFileManager.cpp \
                  RageFileManager_ReadAhead.cpp \
                  RageInput.cpp \
                  RageInputDevice.cpp \
                  RageLog.cpp \
                  RageMath.cpp \
                  RageModelGeometry.cpp \
                  RageSound.cpp \
                  RageSoundManager.cpp \
                  RageSoundMixBuffer.cpp \
                  RageSoundPosMap.cpp \
                  RageSoundReader.cpp \
                  RageSoundReader_Chain.cpp \
                  RageSoundReader_ChannelSplit.cpp \
                  RageSoundReader_Extend.cpp \
                  RageSoundReader_FileReader.cpp \
                  RageSoundReader_MP3.cpp \
                  RageSoundReader_Merge.cpp \
                  RageSoundReader_Pan.cpp \
                  RageSoundReader_PitchChange.cpp \
                  RageSoundReader_PostBuffering.cpp \
                  RageSoundReader_Preload.cpp \
                  RageSoundReader_Resample_Good.cpp \
                  RageSoundReader_SpeedChange.cpp \
                  RageSoundReader_ThreadedBuffer.cpp \
                  RageSoundReader_Vorbisfile.cpp \
                  RageSoundReader_WAV.cpp \
                  RageSoundUtil.cpp \
                  RageSurface.cpp \
                  RageSurfaceUtils.cpp \
                  RageSurfaceUtils_Dither.cpp \
                  RageSurfaceUtils_Palettize.cpp \
                  RageSurfaceUtils_Zoom.cpp \
                  RageSurface_Load.cpp \
                  RageSurface_Load_BMP.cpp \
                  RageSurface_Load_GIF.cpp \
                  RageSurface_Load_JPEG.cpp \
                  RageSurface_Load_PNG.cpp \
                  RageSurface_Load_XPM.cpp \
                  RageSurface_Save_BMP.cpp \
                  RageSurface_Save_JPEG.cpp \
                  RageSurface_Save_PNG.cpp \
                  RageTexture.cpp \
                  RageTextureID.cpp \
                  RageTextureManager.cpp \
                  RageTexturePreloader.cpp \
                  RageTextureRenderTarget.cpp \
                  RageThreads.cpp \
                  RageTimer.cpp \
                  RageTypes.cpp \
                  RageUtil.cpp \
                  RageUtil_BackgroundLoader.cpp \
                  RageUtil_CachedObject.cpp \
                  RageUtil_CharConversions.cpp \
                  RageUtil_FileDB.cpp \
                  RageUtil_WorkerThread.cpp

SCORES_SRC     := ScoreDisplay.cpp \
                  ScoreDisplayAliveTime.cpp \
                  ScoreDisplayBattle.cpp \
                  ScoreDisplayCalories.cpp \
                  ScoreDisplayLifeTime.cpp \
                  ScoreDisplayNormal.cpp \
                  ScoreDisplayOni.cpp \
                  ScoreDisplayPercentage.cpp \
                  ScoreDisplayRave.cpp \
                  ScoreKeeper.cpp \
                  ScoreKeeperNormal.cpp \
                  ScoreKeeperRave.cpp \
                  ScoreKeeperShared.cpp

SCREENS_SRC    :=   Screen.cpp \
                    ScreenAttract.cpp \
                    ScreenBookkeeping.cpp \
                    ScreenContinue.cpp \
                    ScreenDebugOverlay.cpp \
                    ScreenDemonstration.cpp \
                    ScreenEdit.cpp \
                    ScreenEditMenu.cpp \
                    ScreenEnding.cpp \
                    ScreenEvaluation.cpp \
                    ScreenExit.cpp \
                    ScreenGameplay.cpp \
                    ScreenGameplayLesson.cpp \
                    ScreenGameplayNormal.cpp \
                    ScreenGameplayShared.cpp \
                    ScreenGameplaySyncMachine.cpp \
                    ScreenHighScores.cpp \
                    ScreenHowToPlay.cpp \
                    ScreenInstructions.cpp \
                    ScreenJukebox.cpp \
                    ScreenManager.cpp \
                    ScreenMapControllers.cpp \
                    ScreenMessage.cpp \
                    ScreenMiniMenu.cpp \
                    ScreenNameEntry.cpp \
                    ScreenNameEntryTraditional.cpp \
                    ScreenNetEvaluation.cpp \
                    ScreenNetRoom.cpp \
                    ScreenNetSelectBase.cpp \
                    ScreenNetSelectMusic.cpp \
                    ScreenNetworkOptions.cpp \
                    ScreenOptions.cpp \
                    ScreenOptionsCourseOverview.cpp \
                    ScreenOptionsEditCourse.cpp \
                    ScreenOptionsEditProfile.cpp \
                    ScreenOptionsExportPackage.cpp \
                    ScreenOptionsManageCourses.cpp \
                    ScreenDimensions.cpp \
                    ScreenOptionsManageEditSteps.cpp \
                    ScreenOptionsManageProfiles.cpp \
                    ScreenOptionsMaster.cpp \
                    ScreenOptionsMasterPrefs.cpp \
                    ScreenOptionsToggleSongs.cpp \
                    ScreenPackages.cpp \
                    ScreenPlayerOptions.cpp \
                    ScreenProfileLoad.cpp \
                    ScreenProfileSave.cpp \
                    ScreenPrompt.cpp \
                    ScreenRanking.cpp \
                    ScreenReloadSongs.cpp \
                    ScreenSandbox.cpp \
                    ScreenSelect.cpp \
                    ScreenSaveSync.cpp \
                    ScreenServiceAction.cpp \
                    ScreenStatsOverlay.cpp \
                    ScreenSelectCharacter.cpp \
                    ScreenSelectLanguage.cpp \
                    ScreenSelectMaster.cpp \
                    ScreenSelectMusic.cpp \
                    ScreenSelectProfile.cpp \
                    ScreenSyncOverlay.cpp \
                    ScreenSystemLayer.cpp \
                    ScreenSetTime.cpp \
                    ScreenSongOptions.cpp \
                    ScreenSplash.cpp \
                    ScreenTestInput.cpp \
                    ScreenTestLights.cpp \
                    ScreenTestSound.cpp \
                    ScreenTextEntry.cpp \
                    ScreenTitleMenu.cpp \
                    ScreenUnlockBrowse.cpp \
                    ScreenUnlockCelebrate.cpp \
                    ScreenUnlockStatus.cpp \
                    ScreenWithMenuElements.cpp \
                    ScreenInstallOverlay.cpp

#                 ScreenOptionsMemoryCard.cpp \
# Core Sources
STEPMANIA_SRC  += ver.cpp \
                  ezsockets.cpp \
                  ActiveAttackList.cpp \
                  Actor.cpp \
                  ActorFrame.cpp \
                  ActorFrameTexture.cpp \
                  ActorMultiTexture.cpp \
                  ActorProxy.cpp \
                  ActorScroller.cpp \
                  ActorSound.cpp \
                  ActorUtil.cpp \
                  AdjustSync.cpp \
                  AnnouncerManager.cpp \
                  ArrowEffects.cpp \
                  Attack.cpp \
                  AttackDisplay.cpp \
                  AutoActor.cpp \
                  AutoKeysounds.cpp \
                  BGAnimation.cpp \
                  BGAnimationLayer.cpp \
                  BPMDisplay.cpp \
                  Background.cpp \
                  BackgroundUtil.cpp \
                  Banner.cpp \
                  BannerCache.cpp \
                  BeginnerHelper.cpp \
                  BitmapText.cpp \
                  Bookkeeper.cpp \
                  Character.cpp \
                  CharacterManager.cpp \
                  CodeDetector.cpp \
                  CodeSet.cpp \
                  CombinedLifeMeterTug.cpp \
                  ComboGraph.cpp \
                  Command.cpp \
                  CommandLineActions.cpp \
                  CommonMetrics.cpp \
                  ControllerStateDisplay.cpp \
                  Course.cpp \
                  CourseContentsList.cpp \
                  CourseLoaderCRS.cpp \
                  CourseUtil.cpp \
                  CourseWriterCRS.cpp \
                  CreateZip.cpp \
                  CryptManager.cpp \
                  CryptHelpers.cpp \
                  CsvFile.cpp \
                  DancingCharacters.cpp \
                  DateTime.cpp \
                  Difficulty.cpp \
                  DifficultyIcon.cpp \
                  DifficultyList.cpp \
                  DualScrollBar.cpp \
                  DynamicActorScroller.cpp \
                  EditMenu.cpp \
                  EnumHelper.cpp \
                  FadingBanner.cpp \
                  FileDownload.cpp \
                  Font.cpp \
                  FontCharAliases.cpp \
                  FontCharmaps.cpp \
                  FontManager.cpp \
                  Foreground.cpp \
                  Game.cpp \
                  GameCommand.cpp \
                  GameConstantsAndTypes.cpp \
                  GameInput.cpp \
                  GameLoop.cpp \
                  GameManager.cpp \
                  GamePreferences.cpp \
                  GameSoundManager.cpp \
                  GameState.cpp \
                  GameplayAssist.cpp \
                  GhostArrowRow.cpp \
                  Grade.cpp \
                  GradeDisplay.cpp \
                  GraphDisplay.cpp \
                  GrooveRadar.cpp \
                  HelpDisplay.cpp \
                  HighScore.cpp \
                  HoldJudgment.cpp \
                  IniFile.cpp \
                  InputFilter.cpp \
                  InputMapper.cpp \
                  InputQueue.cpp \
                  Inventory.cpp \
                  JsonUtil.cpp \
                  LifeMeter.cpp \
                  LifeMeterBar.cpp \
                  LifeMeterBattery.cpp \
                  LifeMeterTime.cpp \
                  LightsManager.cpp \
                  LocalizedString.cpp \
                  LuaBinding.cpp \
                  LuaExpressionTransform.cpp \
                  LuaManager.cpp \
                  LuaReference.cpp \
                  LyricDisplay.cpp \
                  LyricsLoader.cpp \
                  MemoryCardDisplay.cpp \
                  MemoryCardManager.cpp \
                  MenuTimer.cpp \
                  MessageManager.cpp \
                  MeterDisplay.cpp \
                  ModIcon.cpp \
                  ModIconRow.cpp \
                  Model.cpp \
                  ModelManager.cpp \
                  ModelTypes.cpp \
                  ModsGroup.cpp \
                  MsdFile.cpp \
                  MusicWheel.cpp \
                  MusicWheelItem.cpp \
                  NetworkSyncManager.cpp \
                  NoteData.cpp \
                  NoteDataUtil.cpp \
                  NoteDataWithScoring.cpp \
                  NoteDisplay.cpp \
                  NoteField.cpp \
                  NoteSkinManager.cpp \
                  NoteTypes.cpp \
                  NotesLoader.cpp \
                  NotesLoaderBMS.cpp \
                  NotesLoaderDWI.cpp \
                  NotesLoaderJson.cpp \
                  NotesLoaderKSF.cpp \
                  NotesLoaderSM.cpp \
                  NotesLoaderSMA.cpp \
                  NotesLoaderSSC.cpp \
                  NotesWriterDWI.cpp \
                  NotesWriterJson.cpp \
                  NotesWriterSM.cpp \
                  NotesWriterSSC.cpp \
                  OptionRow.cpp \
                  OptionRowHandler.cpp \
                  OptionsCursor.cpp \
                  OptionsList.cpp \
                  PaneDisplay.cpp \
                  PercentageDisplay.cpp \
                  Player.cpp \
                  PlayerAI.cpp \
                  PlayerNumber.cpp \
                  PlayerOptions.cpp \
                  PlayerStageStats.cpp \
                  PlayerState.cpp \
                  Preference.cpp \
                  PrefsManager.cpp \
                  Profile.cpp \
                  ProfileManager.cpp \
                  Quad.cpp \
                  RadarValues.cpp \
                  $(RAGE_ENGINE) \
                  RandomSample.cpp \
                  ReceptorArrow.cpp \
                  ReceptorArrowRow.cpp \
                  RollingNumbers.cpp \
                  RoomInfoDisplay.cpp \
                  RoomWheel.cpp \
                  SampleHistory.cpp \
                  $(SCREENS_SRC) \
                  $(SCORES_SRC) \
                  ScrollBar.cpp \
                  SnapDisplay.cpp \
                  Song.cpp \
                  SongCacheIndex.cpp \
                  SongManager.cpp \
                  SongOptions.cpp \
                  SongPosition.cpp \
                  SongUtil.cpp \
                  SoundEffectControl.cpp \
                  SpecialFiles.cpp \
                  Sprite.cpp \
                  StageStats.cpp \
                  StatsManager.cpp \
                  StepMania.cpp \
                  Steps.cpp \
                  StepsDisplay.cpp \
                  StepsUtil.cpp \
                  StreamDisplay.cpp \
                  Style.cpp \
                  StyleUtil.cpp \
                  TextBanner.cpp \
                  ThemeManager.cpp \
                  TimingData.cpp \
                  TimingSegments.cpp \
                  TitleSubstitution.cpp \
                  Trail.cpp \
                  TrailUtil.cpp \
                  Transition.cpp \
                  Tween.cpp \
                  UnlockManager.cpp \
                  WheelBase.cpp \
                  WheelItemBase.cpp \
                  WheelNotifyIcon.cpp \
                  WorkoutGraph.cpp \
                  XmlFile.cpp \
                  XmlFileUtil.cpp \
                  global.cpp

LOCAL_SRC_FILES:= $(PLATFORM_SRC) \
                  $(SHARED_ARCH_SRC) \
                  $(STEPMANIA_SRC)

#need to implement these:
                  #archutils/Android/CrashHandler.cpp
                  #arch/MemoryCard/MemoryCardDriver_Android.cpp \


# will need to get these fixed
# can't because networking
#                  ScreenSMOnlineLogin.cpp \
# Unix platform derp.
#                  archutils/Unix/CrashHandlerInternal.cpp \
#                  archutils/Unix/CrashHandlerChild.cpp \
#                  archutils/Unix/AssertionHandler.cpp
#                  archutils/Unix/SignalHandler.cpp \
# Deprecated?
#                  ScreenOptionsReviewWorkout.cpp \

#everything else

LOCAL_C_INCLUDES := $(TOP_PATH) \
                    $(ANDEXTERN_PATH)/ffmpeg

# Currently, we're locking it to ARM
LOCAL_CFLAGS := -DANDROID -DCPU_ARM -DENDIAN_LITTLE -DENDIAN_32BITWORD
LOCAL_CFLAGS += -DANDROID_TEST -finline-limit=300 -fno-exceptions
# Comment out the previous line if we're not doing a testbuild.

LOCAL_LDLIBS    := -llog -landroid -lz -lEGL -lGLESv2 -lOpenSLES -ldl -lm

LOCAL_STATIC_LIBRARIES := jsoncpp libtomcrypt libtommath android_native_app_glue \
                          pcre ndk_helper lua png libmad libvorbis libjpeg \
                          stlport_static

LOCAL_SHARED_LIBRARIES := avcodec avfilter avformat swresample swscale avutil
                          

include $(BUILD_SHARED_LIBRARY)

$(call import-module,$(TOP_PATH)/libtomcrypt)
$(call import-module,$(TOP_PATH)/libtommath)
$(call import-module,$(EXTERN_PATH)/lua-5.1)
$(call import-module,$(EXTERN_PATH)/jsoncpp)
$(call import-module,$(EXTERN_PATH)/libpng)
$(call import-module,$(EXTERN_PATH)/libjpeg)
$(call import-module,$(EXTERN_PATH)/pcre)
# Vorbis brings in libogg by itself
$(call import-module,$(EXTERN_PATH)/android/vorbis)
$(call import-module,$(EXTERN_PATH)/mad-0.15.1b)

#$(call import-module,$(EXTERN_PATH)/glew-1.10)
#$(call import-module,$(EXTERN_PATH)/regal/build/android/Regal/jni)

# Pull FFMpeg in.
include $(ANDEXTERN_PATH)/Android.mk

$(call import-module,android/ndk_helper)
$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
