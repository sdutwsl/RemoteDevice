package icu.sdutwsl.remotedevice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class AudioService extends Service {
    private static final String TAG = AudioService.class.getSimpleName();
    public AudioService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();
        AudioTrackManager am = new AudioTrackManager();
        am.setContext(this.getApplicationContext());
        am.startThread();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
