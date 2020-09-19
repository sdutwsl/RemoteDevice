package icu.sdutwsl.remotedevice;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.Switch;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private ListView mListView;
    private List mAddHeaderFooterList;
    private List mIpAddresses;
    private ArrayAdapter adapter;
    private boolean bTouchpad = false;
    private boolean bAudioService = false;
    private Handler uiHandler = new UIHandler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getSupportActionBar().hide();
        //Bind checked change events for checks
        ((Switch) findViewById(R.id.btn_audio_service)).setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                bAudioService = b;
            }
        });
        ((Switch) findViewById(R.id.btn_touchpad)).setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                bTouchpad = b;
            }
        });
        //Bind data adapter to list component
        mListView = findViewById(R.id.device_list);
        mAddHeaderFooterList = new ArrayList<String>();
        mIpAddresses = new ArrayList<String>();
        adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, mAddHeaderFooterList);
        mListView.setAdapter(adapter);

        //bind click item event to listener
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Intent intent = new Intent();
                intent.putExtra("ip", (String) mIpAddresses.get(i));
                intent.putExtra("name", (String) mAddHeaderFooterList.get(i));
                intent.putExtra("AudioService", bAudioService);
                intent.putExtra("Touchpad", bTouchpad);
                intent.setClass(MainActivity.this, Device.class);
                //禁止返回重选界面
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
                //完成本次活动
                finishActivity(0);
            }
        });
        //Start receive devices' broadcast
        new Thread(new BroadcastReceiver()).start();
    }

    //Update list data to UI
    private class UIHandler extends Handler {
        @Override
        public void handleMessage(@NonNull Message msg) {
            switch (msg.what) {
                case 1:
                    adapter.notifyDataSetChanged();
                    break;
            }
            super.handleMessage(msg);
        }
    }

    //Class to receive udp broadcast and update devices to ui
    private class BroadcastReceiver implements Runnable {
        @Override
        public void run() {
            try {
                DatagramSocket socket = new DatagramSocket(45678);
                byte[] buffer = new byte[1024];
                DatagramPacket dp = new DatagramPacket(buffer, buffer.length);
                while (true) {
                    socket.receive(dp);
                    //Log.d("Ip",dp.getAddress().toString());
                    //修改编码并加入列表
                    if (!mAddHeaderFooterList.contains(new String(dp.getData(), "GB2312") + "💻" + dp.getAddress().getHostAddress().toString())) {
                        mAddHeaderFooterList.add(new String(dp.getData(), "GB2312") + "💻" + dp.getAddress().getHostAddress().toString());
                        mIpAddresses.add(dp.getAddress().getHostAddress().toString());
                    }
                    //修改UI
                    uiHandler.sendEmptyMessage(1);
                }
            } catch (SocketException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }

        }
    }

}