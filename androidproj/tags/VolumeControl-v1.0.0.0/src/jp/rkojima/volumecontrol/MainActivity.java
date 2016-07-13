package jp.rkojima.volumecontrol;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Vibrator;
import android.provider.Settings;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

public class MainActivity extends Activity {

	private void View_setEnabled(int id, boolean enabled) {
		findViewById(id).setEnabled(enabled);
	}

	private void TextView_setText(int id, CharSequence text) {
		((TextView)findViewById(id)).setText(text);
	}

	private void SeekBar_setProgress(int id, int progress) {
		((SeekBar)findViewById(id)).setProgress(progress);
	}
	
	public boolean getVibrateWhenRinging(Context context) {
		Vibrator vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
		if (vibrator == null || !vibrator.hasVibrator()) {
			return false;
		}
		return Settings.System.getInt(this.getApplicationContext().getContentResolver(), "vibrate_when_ringing", 0) != 0;
	}
	
	private void updateAll() {
	
		AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		int volume;

		volume = am.getStreamVolume(AudioManager.STREAM_MUSIC);
		TextView_setText(R.id.tvMusic, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_MUSIC)));
		SeekBar_setProgress(R.id.sbMusic, volume);

		volume = am.getStreamVolume(AudioManager.STREAM_RING);
		TextView_setText(R.id.tvRing, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_RING)));
		SeekBar_setProgress(R.id.sbRing, volume);

		volume = am.getStreamVolume(AudioManager.STREAM_NOTIFICATION);
		TextView_setText(R.id.tvNotification, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_NOTIFICATION)));
		SeekBar_setProgress(R.id.sbNotification, volume);

		volume = am.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
		TextView_setText(R.id.tvVoiceCall, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL)));
		SeekBar_setProgress(R.id.sbVoiceCall, volume);

		volume = am.getStreamVolume(AudioManager.STREAM_ALARM);
		TextView_setText(R.id.tvAlarm, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_ALARM)));
		SeekBar_setProgress(R.id.sbAlarm, volume);

		volume = am.getStreamVolume(AudioManager.STREAM_SYSTEM);
		TextView_setText(R.id.tvSystem, String.format("%d/%d", volume, am.getStreamMaxVolume(AudioManager.STREAM_SYSTEM)));
		SeekBar_setProgress(R.id.sbSystem, volume);
		
		// モード
		ToggleButton tb1, tb2, tb3;
		tb1 = (ToggleButton)findViewById(R.id.tbRingerNormal);
		tb2 = (ToggleButton)findViewById(R.id.tbRingerSilent);
		tb3 = (ToggleButton)findViewById(R.id.tbRingerVibrate);
		switch (am.getRingerMode()) {
		case AudioManager.RINGER_MODE_NORMAL:
			tb1.setChecked(true);
			tb2.setChecked(false);
			tb3.setChecked(false);

			View_setEnabled(R.id.sbSystem, true);
			View_setEnabled(R.id.sbRing, true);
			View_setEnabled(R.id.sbNotification, true);
			View_setEnabled(R.id.btnDownRing, true);
			View_setEnabled(R.id.btnUpRing, true);
			View_setEnabled(R.id.btnDownNotification, true);
			View_setEnabled(R.id.btnUpNotification, true);
			View_setEnabled(R.id.btnDownSystem, true);
			View_setEnabled(R.id.btnUpSystem, true);
			break;
		case AudioManager.RINGER_MODE_SILENT:
			tb1.setChecked(false);
			tb2.setChecked(true);
			tb3.setChecked(false);
			
			View_setEnabled(R.id.sbSystem, false);
			View_setEnabled(R.id.sbRing, false);
			View_setEnabled(R.id.sbNotification, false);
			View_setEnabled(R.id.btnDownRing, false);
			View_setEnabled(R.id.btnUpRing, false);
			View_setEnabled(R.id.btnDownNotification, false);
			View_setEnabled(R.id.btnUpNotification, false);
			View_setEnabled(R.id.btnDownSystem, false);
			View_setEnabled(R.id.btnUpSystem, false);
			break;
		case AudioManager.RINGER_MODE_VIBRATE:
			tb1.setChecked(false);
			tb2.setChecked(false);
			tb3.setChecked(true);

			View_setEnabled(R.id.sbSystem, false);
			View_setEnabled(R.id.sbRing, false);
			View_setEnabled(R.id.sbNotification, false);
			View_setEnabled(R.id.btnDownRing, false);
			View_setEnabled(R.id.btnUpRing, false);
			View_setEnabled(R.id.btnDownNotification, false);
			View_setEnabled(R.id.btnUpNotification, false);
			View_setEnabled(R.id.btnDownSystem, false);
			View_setEnabled(R.id.btnUpSystem, false);
			break;
		}
		
		Vibrator vib = (Vibrator)getSystemService(VIBRATOR_SERVICE);
		if (vib.hasVibrator() == false) {
			tb3.setEnabled(false);
		}
	}
	
	private void AudioManager_declementVolume(int streamType) {
		AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		int n = am.getStreamVolume(streamType);
		if (n > 0) --n;
		am.setStreamVolume(streamType, n, AudioManager.FLAG_PLAY_SOUND);
	}
	
	private void AudioManager_inclementVolume(int streamType) {
		AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		int n = am.getStreamVolume(streamType);
		if (n < am.getStreamMaxVolume(streamType)) ++n;
		am.setStreamVolume(streamType, n, AudioManager.FLAG_PLAY_SOUND);
	}
	
	private void toggleVolume(int seekBarId, int streamType, String keyName) {
		SharedPreferences pref = getSharedPreferences("pref", MODE_PRIVATE);
		SeekBar seekBar = (SeekBar)findViewById(seekBarId);
		AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		if (seekBar.getProgress() == 0) {
			// 保存してある値に復元
			int n = pref.getInt(keyName, -1);
			if (n != -1) {
				manager.setStreamVolume(streamType, n, AudioManager.FLAG_SHOW_UI);
			}
		} else {
			// データを保存して値を0にする
			Editor e = pref.edit();
			e.putInt(keyName, seekBar.getProgress());
			e.commit();
			manager.setStreamVolume(streamType, 0, AudioManager.FLAG_SHOW_UI);
		}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		ImageButton imageButton;
		imageButton = (ImageButton)findViewById(R.id.btnDownMusic);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_declementVolume(AudioManager.STREAM_MUSIC);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpMusic);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_MUSIC);
				updateAll();
			}
		});
		
		imageButton = (ImageButton)findViewById(R.id.btnDownRing);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				int n = am.getStreamVolume(AudioManager.STREAM_RING);
				if (n > 1) --n;
				else Toast.makeText(getApplicationContext(), "ボリュームを 0 にしたいときは、Silent/Vibrateを選択してください", Toast.LENGTH_SHORT).show();
				am.setStreamVolume(AudioManager.STREAM_RING, n, AudioManager.FLAG_PLAY_SOUND);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpRing);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_RING);
				updateAll();
			}
		});

		imageButton = (ImageButton)findViewById(R.id.btnDownNotification);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				int n = am.getStreamVolume(AudioManager.STREAM_NOTIFICATION);
				if (n > 1) --n;
				else Toast.makeText(getApplicationContext(), "ボリュームを 0 にしたいときは、Silent/Vibrateを選択してください", Toast.LENGTH_SHORT).show();
				am.setStreamVolume(AudioManager.STREAM_NOTIFICATION, n, AudioManager.FLAG_PLAY_SOUND);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpNotification);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_NOTIFICATION);
				updateAll();
			}
		});
		
		imageButton = (ImageButton)findViewById(R.id.btnDownVoiceCall);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				int n = am.getStreamVolume(AudioManager.STREAM_VOICE_CALL);
				if (n > 0) --n;
				am.setStreamVolume(AudioManager.STREAM_VOICE_CALL, n, AudioManager.FLAG_PLAY_SOUND);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpVoiceCall);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_VOICE_CALL);
				updateAll();
			}
		});

		imageButton = (ImageButton)findViewById(R.id.btnDownAlarm);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				int n = am.getStreamVolume(AudioManager.STREAM_ALARM);
				if (n > 0) --n;
				am.setStreamVolume(AudioManager.STREAM_ALARM, n, AudioManager.FLAG_PLAY_SOUND);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpAlarm);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_ALARM);
				updateAll();
			}
		});
		
		imageButton = (ImageButton)findViewById(R.id.btnDownSystem);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				int n = am.getStreamVolume(AudioManager.STREAM_SYSTEM);
				if (n > 0) --n;
				am.setStreamVolume(AudioManager.STREAM_SYSTEM, n, AudioManager.FLAG_PLAY_SOUND);
				updateAll();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnUpSystem);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager_inclementVolume(AudioManager.STREAM_SYSTEM);
				updateAll();
			}
		});
		
		imageButton = (ImageButton)findViewById(R.id.btnToggleMusic);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbMusic, AudioManager.STREAM_MUSIC, "VolumeMusic");
				finish();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnToggleRing);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbRing, AudioManager.STREAM_RING, "VolumeRing");
				finish();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnToggleNotification);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbNotification, AudioManager.STREAM_NOTIFICATION, "VolumeNotification");
				finish();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnToggleVoiceCall);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbVoiceCall, AudioManager.STREAM_VOICE_CALL, "VolumeVoiceCall");
				finish();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnToggleAlarm);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbAlarm, AudioManager.STREAM_ALARM, "VolumeAlarm");
				finish();
			}
		});
		imageButton = (ImageButton)findViewById(R.id.btnToggleSystem);
		imageButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				toggleVolume(R.id.sbSystem, AudioManager.STREAM_SYSTEM, "VolumeSystem");
				finish();
			}
		});
		
		// Listenerを登録する前にSeekBarを初期化する
		AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
		
		SeekBar seekBar;
		seekBar = (SeekBar)findViewById(R.id.sbMusic);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_MUSIC));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
					manager.setStreamVolume(AudioManager.STREAM_MUSIC, progress, AudioManager.FLAG_PLAY_SOUND);
					updateAll();
				}
			}
		});

		seekBar = (SeekBar)findViewById(R.id.sbRing);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_RING));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					if (progress > 0) {
						AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
						manager.setStreamVolume(AudioManager.STREAM_RING, progress, AudioManager.FLAG_PLAY_SOUND);
						updateAll();
					} else {
						seekBar.setProgress(1);
						//Toast.makeText(getApplicationContext(), "ボリュームを 0 にしたいときは、Silent/Vibrateを選択してください", Toast.LENGTH_SHORT).show();
					}
				}
			}
		});

		seekBar = (SeekBar)findViewById(R.id.sbNotification);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_NOTIFICATION));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					if (progress > 0) {
						AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
						manager.setStreamVolume(AudioManager.STREAM_NOTIFICATION, progress, AudioManager.FLAG_PLAY_SOUND);
						updateAll();
					} else {
						seekBar.setProgress(1);
						//Toast.makeText(getApplicationContext(), "ボリュームを 0 にしたいときは、Silent/Vibrateを選択してください", Toast.LENGTH_SHORT).show();
					}
				}
			}
		});
		
		seekBar = (SeekBar)findViewById(R.id.sbVoiceCall);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_VOICE_CALL));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
					manager.setStreamVolume(AudioManager.STREAM_VOICE_CALL, progress, AudioManager.FLAG_PLAY_SOUND);
					updateAll();
				}
			}
		});

		seekBar = (SeekBar)findViewById(R.id.sbAlarm);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_ALARM));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
					manager.setStreamVolume(AudioManager.STREAM_ALARM, progress, AudioManager.FLAG_PLAY_SOUND);
					updateAll();
				}
			}
		});

		seekBar = (SeekBar)findViewById(R.id.sbSystem);
		seekBar.setMax(manager.getStreamMaxVolume(AudioManager.STREAM_SYSTEM));
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser) {
					AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
					manager.setStreamVolume(AudioManager.STREAM_SYSTEM, progress, AudioManager.FLAG_PLAY_SOUND);
					updateAll();
				}
			}
		});
		
		ToggleButton tb;
		tb = (ToggleButton)findViewById(R.id.tbRingerNormal);
		tb.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				manager.setRingerMode(AudioManager.RINGER_MODE_NORMAL);
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				updateAll();
			}
		});
		tb = (ToggleButton)findViewById(R.id.tbRingerSilent);
		tb.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				manager.setRingerMode(AudioManager.RINGER_MODE_SILENT);
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				updateAll();
			}
		});
		tb = (ToggleButton)findViewById(R.id.tbRingerVibrate);
		tb.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				AudioManager manager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
				manager.setRingerMode(AudioManager.RINGER_MODE_VIBRATE);
				try {
					Thread.sleep(200);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				updateAll();
			}
		});
		
		updateAll();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			PackageInfo pi;
			try {
				pi = getPackageManager().getPackageInfo("jp.rkojima.volumecontrol", PackageManager.GET_META_DATA);
				Toast.makeText(getApplicationContext(), 
						String.format("version %s", pi.versionName), Toast.LENGTH_SHORT).show();
			} catch (NameNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
