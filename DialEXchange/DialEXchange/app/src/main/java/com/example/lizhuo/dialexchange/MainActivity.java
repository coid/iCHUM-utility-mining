package com.example.lizhuo.dialexchange;

import android.app.PendingIntent;
import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.List;


public class MainActivity extends ActionBarActivity {

    private Button cancel;
 //   private Button notify;
    private Button zjp;
    private Button gs;
    private Button lfb;
    private Button lz;
    private Button wzl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initComponent();
    }

    public void initComponent(){
        cancel = (Button)findViewById(R.id.cancel);
     //   notify=  (Button)findViewById(R.id.notify);
        zjp = (Button)findViewById(R.id.person1);
        gs = (Button)findViewById(R.id.person2);
        lfb = (Button)findViewById(R.id.person3);
        lz = (Button)findViewById(R.id.person4);
        wzl = (Button)findViewById(R.id.person5);

        final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";

        cancel.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL", Uri.parse("tel:*720"));
                startActivity(phoneIntent);
            }
        });

//        notify.setOnClickListener(new View.OnClickListener(){
//            @Override
//            public void onClick(View v) {
//                try{
//                    Intent phoneIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:15920064434"));
//                    phoneIntent.putExtra("sms_body", sms);
//                    startActivity(phoneIntent);
////                    String number = "15920064434";
////                    String neirong = "hello";
////                SmsManager smsManager = SmsManager.getDefault();
////                    List<String> texts =smsManager.divideMessage(neirong);
////                    PendingIntent sentPI;
////                    sentPI = PendingIntent.getBroadcast(MainActivity.this, 0,new Intent(), 0);
////                    PendingIntent deliveredPI = PendingIntent.getBroadcast(MainActivity.this, 0,new Intent(), 0);
////
////                smsManager.sendTextMessage(number, null, neirong, sentPI, null);
//                    Toast.makeText(getApplicationContext(),
//                            "SMS sent!",
//                            Toast.LENGTH_LONG).show();
//                }catch (Exception e){
//                    Toast.makeText(getApplicationContext(),
//                            "SMS faild, please try again later!",
//                            Toast.LENGTH_LONG).show();
//
//                    e.printStackTrace();
//                }
//            }
//        });

        zjp.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7215920079679"));
                startActivity(phoneIntent);
                Intent smsIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:15920079679"));
                smsIntent.putExtra("sms_body", sms);
                startActivity(smsIntent);
            }
        });

        gs.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7218682285868"));
                startActivity(phoneIntent);
                Intent smsIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:18682285868"));
                smsIntent.putExtra("sms_body", sms);
                startActivity(smsIntent);
            }
        });

        lfb.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7213126568928"));
                startActivity(phoneIntent);
                Intent smsIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:13126568928"));
                smsIntent.putExtra("sms_body", sms);
                startActivity(smsIntent);
            }
        });

        lz.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7215920064434"));
                startActivity(phoneIntent);
                Intent smsIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:15920064434"));
                smsIntent.putExtra("sms_body", sms);
                startActivity(smsIntent);
            }
        });

        wzl.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                final String sms = "您好,本周您将负责网络中心接话任务,本机会在转发电话到您的手机上,此行为将至少维持一个星期.如果逾期还接到转接电话,请及时联系李卓同学.";
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7218588215892"));
                startActivity(phoneIntent);
                Intent smsIntent = new Intent("android.intent.action.SENDTO", Uri.parse("smsto:18588215892"));
                smsIntent.putExtra("sms_body", sms);
                startActivity(smsIntent);
            }
        });
    }

}
