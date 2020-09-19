package icu.sdutwsl.remotedevice;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;

import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;

public class Device extends AppCompatActivity {
    public boolean bAudioService = false;
    public boolean bTouchpad = false;
    public String destIP = "";
    public String destHostname = "";
    public int destPort = 19972;
    public TouchpadSender tTouchPad;
    //private boolean bLButtonDown=false;
    GestureDetector gestureDetector;

    @RequiresApi(api = Build.VERSION_CODES.O)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device);
        //删除标题栏 hide action bar
        getSupportActionBar().hide();
        gestureDetector = new GestureDetector(this, new MyGestureHandler());
        //触控监听
        ((LinearLayout) findViewById(R.id.layout_touchpad)).setOnTouchListener(new View.OnTouchListener() {
            private float oldX = 0;
            private float oldY = 0;

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        oldX = motionEvent.getX();
                        oldY = motionEvent.getY();
                        // 处理输入的按下事件
                        //Log.e("DOWN","DOWN");
                        break;

                    case MotionEvent.ACTION_MOVE:
                        // 处理输入的移动 MOUSE_MOVE
                        //Log.e("Move",(motionEvent.getX()-oldX)+"   "+(motionEvent.getY()-oldY));
                        float deltaX = motionEvent.getX() - oldX;
                        float deltaY = motionEvent.getY() - oldY;
                        tTouchPad.setSend("M#" + (int) (deltaX) + "#" + (int) (deltaY));
                        oldX = motionEvent.getX();
                        oldY = motionEvent.getY();
                        break;
                    //每次都发送一次LBUTTON_UP
                    case MotionEvent.ACTION_UP:
                        Log.d("Pc", motionEvent.getPointerCount() + "");
                        // 处理输入的离开事件
                        //双指则是左键双击
                        break;

                }
                return gestureDetector.onTouchEvent(motionEvent);
            }
        });
        //垂直滚动条
        ((LinearLayout) findViewById(R.id.layout_vscroll)).setOnTouchListener(new View.OnTouchListener() {
            private float oldY;

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        oldY = motionEvent.getY();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        float delta = motionEvent.getY() - oldY;
                        int i = (int) (delta / Math.abs(delta));
                        tTouchPad.setSend("SV#" + i);
                        break;
                }
                return true;
            }
        });
        //水平滚动条
        ((LinearLayout) findViewById(R.id.layout_hscroll)).setOnTouchListener(new View.OnTouchListener() {
            private float oldX;

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        oldX = motionEvent.getX();
                        break;
                    case MotionEvent.ACTION_MOVE:
                        float delta = motionEvent.getX() - oldX;
                        int i = (int) (delta / Math.abs(delta));
                        tTouchPad.setSend("SH#" + i);
                        break;
                }
                return true;
            }
        });
        //左右键
        ((Button) findViewById(R.id.btn_lbutton)).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        tTouchPad.setSend("LD");
                        break;
                    case MotionEvent.ACTION_UP:
                        tTouchPad.setSend("LU");
                        break;
                }
                return true;
            }
        });
        ((Button) findViewById(R.id.btn_rbutton)).setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        tTouchPad.setSend("RD");
                        break;
                    case MotionEvent.ACTION_UP:
                        tTouchPad.setSend("RU");
                        break;
                }
                return true;
            }
        });
        Intent intent = getIntent();
        //获取主机名与ip地址 get ip and hostname from intent
        this.destHostname = intent.getStringExtra("name");
        this.destIP = intent.getStringExtra("ip");
        Log.d("IP", this.destIP);
        this.bTouchpad = intent.getBooleanExtra("Touchpad", false);
        this.bAudioService = intent.getBooleanExtra("AudioService", false);
        new Thread(new KeepAlive()).start();
        if (bTouchpad == true) {
            tTouchPad = new TouchpadSender();
            new Thread(tTouchPad).start();
        }
        if (bAudioService == true) {
            Intent as = new Intent(this, AudioService.class);
            startForegroundService(as);
        }
    }

    private class MyGestureHandler implements GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {

        //左键单击 LBUTTON_CLICK
        @Override
        public boolean onSingleTapConfirmed(MotionEvent motionEvent) {
            Log.e("LBUTTON", "CLICK");
            tTouchPad.setSend("Lc");
            return true;
        }

        //左键按下
        @Override
        public boolean onDoubleTap(MotionEvent motionEvent) {
            Log.e("LBUTTON", "DBCLICK");
            tTouchPad.setSend("LC");
            return true;
        }

        //左键抬起
        @Override
        public boolean onDoubleTapEvent(MotionEvent motionEvent) {
            //Log.e("LBUTTON","DOUBLECLICK");
            return true;
        }

        @Override
        public boolean onDown(MotionEvent motionEvent) {
            return true;
        }

        @Override
        public void onShowPress(MotionEvent motionEvent) {
            //Log.e("LBUTTON","DBCLICK");
        }

        @Override
        public boolean onSingleTapUp(MotionEvent motionEvent) {
            return true;
        }

        @Override
        public boolean onScroll(MotionEvent motionEvent, MotionEvent motionEvent1, float v, float v1) {

            return true;
        }

        @Override
        public void onLongPress(MotionEvent motionEvent) {
            Log.e("RBUTTON", "CLICK");
            tTouchPad.setSend("RC");
        }

        //这里是MOUSE_MOVE
        @Override
        public boolean onFling(MotionEvent motionEvent, MotionEvent motionEvent1, float v, float v1) {
            return true;
        }
    }

    private class TouchpadSender implements Runnable {
        private boolean bSend = false;
        private String strSend = "";
        DatagramSocket ds;
        DatagramPacket dp;

        public synchronized void setSend(String str) {
            this.strSend = str;
            bSend = true;
        }

        @Override
        public void run() {
            try {
                ds = new DatagramSocket();
                dp = new DatagramPacket(strSend.getBytes("GBK"), strSend.getBytes("GBK").length, new InetSocketAddress(destIP, 19973));
            } catch (SocketException | UnsupportedEncodingException e) {
                e.printStackTrace();
            }
            while (true) {
                if (bSend&&bTouchpad) {
                    try {
                        //Log.e("Send:   ",strSend+"  aim:"+dp.getAddress().toString());
                        dp.setData(strSend.getBytes("GBK"), 0, strSend.getBytes("GBK").length);
                        ds.send(dp);
                        bSend = false;
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                        bSend = false;
                    } catch (IOException e) {
                        bSend = false;
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    private class KeepAlive implements Runnable {
        public Socket sockKeepAlive;

        @Override
        public void run() {
            //Create tcp socket
            try {
                sockKeepAlive = new Socket(destIP, destPort);
                OutputStream os = sockKeepAlive.getOutputStream();
                os.write(("Touchpad:" + bTouchpad).getBytes("GBK"));
                os.flush();
                Thread.sleep(100);
                os.write(("Audio:" + bAudioService).getBytes("GBK"));
                Thread.sleep(100);
                os.flush();
                while (true) {
                    os.write("Alive".getBytes("GBK"));
                    Thread.sleep(100);
                }
            } catch (IOException | InterruptedException e) {
                finish();
                e.printStackTrace();

            }
        }
    }
}