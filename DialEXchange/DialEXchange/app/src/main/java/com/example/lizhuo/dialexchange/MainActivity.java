package com.example.lizhuo.dialexchange;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;


public class MainActivity extends ActionBarActivity {

    private Button cancel;
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
        zjp = (Button)findViewById(R.id.person1);
        gs = (Button)findViewById(R.id.person2);
        lfb = (Button)findViewById(R.id.person3);
        lz = (Button)findViewById(R.id.person4);
        wzl = (Button)findViewById(R.id.person5);

        cancel.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL", Uri.parse("tel:*720"));
                startActivity(phoneIntent);
            }
        });

        zjp.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7215920079679"));
                startActivity(phoneIntent);
            }
        });

        gs.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7218682285868"));
                startActivity(phoneIntent);
            }
        });

        lfb.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7213126568928"));
                startActivity(phoneIntent);
            }
        });

        lz.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7215920064434"));
                startActivity(phoneIntent);
            }
        });

        wzl.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent phoneIntent = new Intent("android.intent.action.CALL",Uri.parse("tel:*7218588215892"));
                startActivity(phoneIntent);
            }
        });
    }

}
