package icu.sdutwsl.remotedevice;

import android.content.Context;
import android.media.AudioAttributes;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Environment;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class AudioTrackManager {

    private static final String TAG = AudioTrack.class.getSimpleName();

    public static String PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + "/a.pcm";

    private static AudioTrackManager instance;

    private int mSampleRate = 48000;

    private int channelCount = 2;

    private AudioTrack mAudioTrack;

    private int channelConfig = AudioFormat.CHANNEL_OUT_STEREO;

    private int bufferSize;

    private int audioFormatEncode = AudioFormat.ENCODING_PCM_FLOAT;

    private AudioManager mAudioManager;

    private Context mContext;

    private AudioFormat mAudioFormat;

    private FileInputStream fileInputStream;

    private boolean startPlay = false;

    public AudioTrackManager() {

    }

    public static AudioTrackManager getInstance() {
        if (instance == null) {
            instance = new AudioTrackManager();
        }
        return instance;
    }

    public void setContext(Context context) {
        this.mContext = context;
        init();
    }

    private void init() {
        mAudioManager = (AudioManager) mContext.getSystemService(Context.AUDIO_SERVICE);
        bufferSize = AudioTrack.getMinBufferSize(mSampleRate, channelConfig, audioFormatEncode);
        int sessionId = mAudioManager.generateAudioSessionId();
        AudioAttributes audioAttributes = new AudioAttributes.Builder()
                .setUsage(AudioAttributes.USAGE_MEDIA)
                .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                .build();
        AudioFormat audioFormat = new AudioFormat.Builder().setSampleRate(mSampleRate)
                .setEncoding(audioFormatEncode)
                .setChannelMask(channelConfig)
                .build();
        mAudioTrack = new AudioTrack(audioAttributes, audioFormat, bufferSize * 2, AudioTrack.MODE_STREAM, sessionId);
    }

    public void startThread() {
        startPlay = true;
        new PlayThread().start();
    }

    public void stopThread() {
        startPlay = false;
        mAudioTrack.stop();
        mAudioTrack.release();
    }
    public static float[] byteArrayToFloatArray(byte[] bytes,int rlength) {
        ByteBuffer buffer = ByteBuffer.wrap(bytes,0,rlength);
        FloatBuffer fb = buffer.order(ByteOrder.LITTLE_ENDIAN).asFloatBuffer();
        float[] floatArray = new float[fb.limit()];
        fb.get(floatArray);
        return floatArray;
    }
    class PlayThread extends Thread {
        @Override
        public void run() {
            super.run();
            //ServerSocket s;
            DatagramSocket ds;

            try {
                //s=new ServerSocket(11600);
                //Socket client=s.accept();
                //DataInputStream dis = new DataInputStream(
                //        client.getInputStream());
                ds=new DatagramSocket(11600);
                byte []rb=new byte[65535];
                DatagramPacket dp=new DatagramPacket(rb,rb.length);
                mAudioTrack.play();
                //int count=0;
                while(true) {
                    //dis.read(rb);
                    ds.receive(dp);
                    //Log.d("Recv data len=",""+rs);
                    float []fb=byteArrayToFloatArray(dp.getData(),dp.getLength());
                    mAudioTrack.write(fb,0,fb.length,AudioTrack.WRITE_NON_BLOCKING);
                }
            } catch (SocketException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


}