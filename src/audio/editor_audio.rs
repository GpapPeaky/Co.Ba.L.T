use kira::{
    {AudioManager, AudioManagerSettings},
    sound::static_sound::{StaticSoundData, StaticSoundSettings},
};
use rand::prelude::*;
use std::sync::Arc;

use crate::options::editor_options::EditorOptions;

/// Struct holding all editor sound effects
pub struct EditorAudio {
    pub manager: AudioManager,
    pub insert: Arc<StaticSoundData>,
    pub delete: Arc<StaticSoundData>,
    pub space: Arc<StaticSoundData>,
    pub enter: Arc<StaticSoundData>,
    pub nav: Arc<StaticSoundData>,
}

impl EditorAudio {
    /// Load all sounds and create AudioManager
    pub fn new() -> Self {
        let manager = AudioManager::new(AudioManagerSettings::default())
            .expect("Failed to create AudioManager");

        let insert = Arc::new(
            StaticSoundData::from_file("assets/sound/insert.wav").expect("insert.wav missing"),
        );
        let delete = Arc::new(
            StaticSoundData::from_file("assets/sound/del.wav").expect("del.wav missing"),
        );
        let enter = Arc::new(
            StaticSoundData::from_file("assets/sound/return.wav").expect("return.wav missing"),
        );
        let space = Arc::new(
            StaticSoundData::from_file("assets/sound/space.wav").expect("space.wav missing"),
        );
        let nav = Arc::new(
            StaticSoundData::from_file("assets/sound/nav.wav").expect("nav.wav missing"),
        );

        Self {
            manager,
            insert,
            delete,
            space,
            enter,
            nav,
        }
    }

    /// Generates a subtle ±5% pitch variation
    fn random_pitch() -> f64 {
        let mut rng = rand::rng();
        let pitch = 1.0 + rng.random_range(-0.075..0.075);
        pitch
    }

    /// Plays a sound with pitch and volume variation
    fn play_static_sound(&mut self,
        sound: Arc<StaticSoundData>,
        op: &EditorOptions
    ) {
        if !op.audio { return; } // No Audio

        let mut settings = StaticSoundSettings::default();
        settings.volume = (-20.0).into();
        settings.playback_rate = Self::random_pitch().into();

        let sound_data = (*sound).clone().with_settings(settings);
        self.manager.play(sound_data).expect("Failed to play sound");
    }

    pub fn play_insert(
        &mut self,
        op: &EditorOptions
    ) {
        self.play_static_sound(Arc::clone(&self.insert), op);
    }

    pub fn play_delete(
        &mut self,
        op: &EditorOptions
    ) {
        self.play_static_sound(Arc::clone(&self.delete), op);
    }

    pub fn play_space(
        &mut self,
        op: &EditorOptions
    ) {
        self.play_static_sound(Arc::clone(&self.space), op);
    }

    pub fn play_return(
        &mut self,
        op: &EditorOptions
    ) {
        self.play_static_sound(Arc::clone(&self.enter), op);
    }

    pub fn play_nav(
        &mut self,
        op: &EditorOptions
    ) {
        self.play_static_sound(Arc::clone(&self.nav), op);
    }
}
