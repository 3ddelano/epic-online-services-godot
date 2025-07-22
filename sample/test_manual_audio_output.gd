# This script is an example of using EOS manual audio output and input.
# In this sample, user2 will automatically create a lobby
# and user1 will join that lobby.
# Now once in the lobby:
#    user2's voice will be sent via EOS to user1.
#    And user2's voice will be heard by user1
#    using manual audio output.
#    Audio output is muted for user2 to prevent loopback so
#    that you can test on the same device.
#
# In user2's window, hold T to speak.
# The audio should be heard in user1's window.
#
# To run this sample, uncomment the _run() line in _ready() method
extends Node


# Provide 2 different Product User IDs for testing
var user1_puid = ""
var user2_puid = Store.second_product_user_id


const BUCKET_ID = "test_manual_audio_output"


var _lobby: HLobby
var _connected_lobby_signals = false
var _rtc_room_name = ""

var player_voices = {}


# for audio input
var _audio_input: AudioStreamPlayer
var _audio_effect_capture: AudioEffectCapture
var _audio_bus_index: int
const SAMPLE_RATE = 48000
const CHANNELS = 1
const CHUNK_DURATION_MS = 10  # 10 milliseconds
const FRAMES_PER_CHUNK = int(SAMPLE_RATE * CHUNK_DURATION_MS / 1000.0)  # 480 frames

# Recording state
var is_recording = false
var audio_buffer = PackedFloat32Array()

# Voice activity detection (optional)
var voice_threshold = 0.01  # Adjust based on your needs
var silence_frames = 0
var max_silence_frames = 10  # Stop recording after this many silent chunks


func _ready():
	# Uncomment the below line to run this sample
	_run()
	pass


func _run():
	IEOS.rtc_audio_audio_before_render.connect(_on_rtc_audio_audio_before_render)
	
	await HAuth.logged_in
	
	if is_user2():
		_setup_audio_capture()
	
	print("Running the EOS manual audio output sample")
	
	HLobbies.local_rtc_options.use_manual_audio_output = true
	
	if EOSGRuntime.local_product_user_id == user2_puid:
		_run_user2()
	else:
		_run_user1()


func _run_user1():
	_log("Running as user1")
	
	_log("waiting for lobby to be created")
	await get_tree().create_timer(5).timeout
	
	_log("searching for lobbies now")
	var lobbies: Array = await HLobbies.search_by_product_user_id_async(user2_puid)
	
	_log("found lobbies count: ", len(lobbies))
	if len(lobbies) > 0:
		var most_recent: HLobby = lobbies.back()
		_lobby = await HLobbies.join_async(most_recent)
		_log("joined lobby with id ", most_recent.lobby_id)
		_on_lobby_joined()


func _run_user2():
	_log("Running as user2")
	
	var create_opts := EOS.Lobby.CreateLobbyOptions.new()
	create_opts.bucket_id = "test_manual_audio_output"
	create_opts.max_lobby_members = 4
	create_opts.presence_enabled = true
	create_opts.enable_rtc_room = true
	create_opts.permission_level = EOS.Lobby.LobbyPermissionLevel.PublicAdvertised
	create_opts.allow_invites = true
	create_opts.local_rtc_options = {
		flags = EOS.RTC.JoinRoomFlags.EnableDataChannel,
		use_manual_audio_input = true,
		use_manual_audio_output = true,
	}
	_lobby = await HLobbies.create_lobby_async(create_opts)
	_log("created lobby with id ", _lobby.lobby_id)
	_on_lobby_joined()


func _on_lobby_joined():
	_lobby.lobby_updated.connect(_on_lobby_updated)


func _on_lobby_updated():
	if _lobby.rtc_room_name and not _connected_lobby_signals:
		_connected_lobby_signals = true
		_connect_rtc_signals()


func _connect_rtc_signals():
	_rtc_room_name = _lobby.rtc_room_name
	_log("Got rtc room name: ", _rtc_room_name)

	if not is_user2():
		_log("setting up manual audio output for user1")
		var opts = EOS.RTCAudio.AddNotifyAudioBeforeRenderOptions.new()
		opts.unmixed_audio = true
		opts.room_name = _rtc_room_name
		EOS.RTCAudio.RTCAudioInterface.add_notify_audio_before_render(opts)
		
		# create the audio stream player node to hear user2's voice
		
		# AudioStreamGenerator for real-time audio streaming
		var stream_generator = AudioStreamGenerator.new()
		stream_generator.mix_rate = 48000 # 48000 is what EOS gives
		stream_generator.buffer_length = 0.01 # 10ms buffer is what EOS gives
		
		var audio_player = AudioStreamPlayer.new()
		add_child(audio_player)
		audio_player.stream = stream_generator
		audio_player.autoplay = true
		audio_player.play()
		
		player_voices[user2_puid] = {
			audio_player = audio_player,
			playback = audio_player.get_stream_playback()
		}


func _setup_audio_capture():
	_log("Setting up capturing microphone audio")
	# Get the microphone input bus
	_audio_bus_index = AudioServer.get_bus_index("Microphone")
	# Get the audio effect capture
	_audio_effect_capture = AudioServer.get_bus_effect(_audio_bus_index, 0)
	
	# Create AudioStreamPlayer for microphone input
	_audio_input = AudioStreamPlayer.new()
	add_child(_audio_input)
	
	# Set up microphone input
	var microphone_stream = AudioStreamMicrophone.new()
	_audio_input.stream = microphone_stream
	_audio_input.bus = "Microphone"


func _on_rtc_audio_audio_before_render(data: Dictionary):
	var puid = data.participant_id
	if puid in player_voices:
		#_log("got audio before render: for player: ", puid)
		var buffer_data = data.buffer
		var playback = player_voices[puid].playback
		
		# Convert the frame data to audio samples
		var frames = buffer_data["frames"]
		var sample_rate = buffer_data["sample_rate"]
		var channels = buffer_data["channels"]
		
		# Create PackedVector2Array for audio data (required by AudioStreamGeneratorPlayback)
		var audio_data = PackedVector2Array()
		
		# Convert integer frames to float samples (normalize to -1.0 to 1.0 range)
		# Assuming the frames are 16-bit integers (-32768 to 32767)
		for frame in frames:
			var sample = float(frame) / 32768.0
			# For mono audio, use the same sample for both left and right channels
			audio_data.append(Vector2(sample, sample))
		
		# Push the audio data to the stream
		playback.push_buffer(audio_data)


func _process(_delta: float):
	if not is_user2():
		return
	
	if not is_recording:
		return
	
	# Get available audio frames
	var available_frames = _audio_effect_capture.get_frames_available()
	
	if available_frames >= FRAMES_PER_CHUNK:
		# Capture audio chunk
		var audio_data = _audio_effect_capture.get_buffer(FRAMES_PER_CHUNK)
		process_audio_chunk(audio_data)


func start_voice_recording():
	if is_recording or not _audio_input:
		return
	
	is_recording = true
	audio_buffer.clear()
	
	# Start microphone recording
	_audio_input.play()
	
	# Enable audio capture
	_audio_effect_capture.clear_buffer()
	_log("Voice recording started")


func stop_voice_recording():
	if not is_recording or not _audio_input:
		return
	
	is_recording = false
	_audio_input.stop()
	
	_log("Voice recording stopped")


func process_audio_chunk(audio_data: PackedVector2Array):
	# Convert stereo Vector2 data to mono float array
	var mono_samples = PackedFloat32Array()
	
	for sample in audio_data:
		# Convert stereo to mono by averaging left and right channels
		var mono_sample = (sample.x + sample.y) / 2.0
		mono_samples.append(mono_sample)
	
	# Convert to 16-bit integer format
	var frames_16bit = convert_to_16bit(mono_samples)
		
	# Create voice data dictionary
	var voice_data = {
		"buffer": {
			"frames_count": frames_16bit.size(),
			"frames": frames_16bit,
			"sample_rate": SAMPLE_RATE,
			"channels": CHANNELS
		}
	}
	
	# Broadcast the voice data
	# _log("sending voice data", voice_data)


func convert_to_16bit(float_samples: PackedFloat32Array) -> Array:
	var int_samples = []
	
	for sample in float_samples:
		# Clamp sample to [-1.0, 1.0] range
		sample = clamp(sample, -1.0, 1.0)
		
		# Convert to 16-bit integer (-32768 to 32767)
		var int_sample = int(sample * 32767.0)
		int_samples.append(int_sample)
	
	return int_samples


func has_voice_activity(samples: PackedFloat32Array) -> bool:
	# Simple voice activity detection based on RMS (Root Mean Square)
	var rms = 0.0
	
	for sample in samples:
		rms += sample * sample
	
	rms = sqrt(rms / samples.size())
	
	return rms > voice_threshold


func _input(event: InputEvent) -> void:
	if Input.is_action_just_pressed(&"push_to_talk"):
		start_voice_recording()
	if Input.is_action_just_released(&"push_to_talk"):
		stop_voice_recording()


#region internals

func _log_only_user1(v0, v1 = null):
	_log(v0, v1, user1_puid)


func _log_only_user2(v0, v1 = null):
	_log(v0, v1, user2_puid)


func is_user2() -> bool:
	return HAuth.product_user_id == user2_puid

func _log(v0, v1 = null, only_user_with_puid = null):
	var user_num = "2" if is_user2() else "1"
	
	if only_user_with_puid and only_user_with_puid != HAuth.product_user_id:
		return
	
	if v1 == null:
		print("user:: " + str(user_num) + ":: ", v0)
	else:
		print("user:: " + str(user_num) + ":: ", v0, v1)

#endregion
