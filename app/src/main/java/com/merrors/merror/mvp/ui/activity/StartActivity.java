package com.merrors.merror.mvp.ui.activity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.jess.arms.base.BaseActivity;
import com.jess.arms.di.component.AppComponent;
import com.jess.arms.utils.UiUtils;
import com.merrors.merror.app.arouter.ARouterPaths;
import com.merrors.merror.di.component.DaggerStartComponent;
import com.merrors.merror.di.module.StartModule;
import com.merrors.merror.mvp.contract.StartContract;
import com.merrors.merror.mvp.presenter.StartPresenter;
import com.merrors.merror.R;
import com.tbruyelle.rxpermissions2.RxPermissions;
import static com.jess.arms.utils.Preconditions.checkNotNull;

@Route(path = ARouterPaths.StartActivity)
public class StartActivity extends BaseActivity<StartPresenter> implements StartContract.View {

    private RxPermissions mRxPermissions;

    @Override
    public void setupActivityComponent(AppComponent appComponent) {
        this.mRxPermissions = new RxPermissions(this);
        mPresenter.requestPermissions();
        DaggerStartComponent //如找不到该类,请编译一下项目
                .builder()
                .appComponent(appComponent)
                .startModule(new StartModule(this))
                .build()
                .inject(this);
    }

    @Override
    public int initView(Bundle savedInstanceState) {
        return R.layout.activity_start; //如果你不需要框架帮你设置 setContentView(id) 需要自行设置,请返回 0
    }

    @Override
    public void initData(Bundle savedInstanceState) {
        //目前不管都先跳轉,以後在想要做啥
        mPresenter.gotoContainAvtivity(this);
    }


    @Override
    public void showLoading() {

    }

    @Override
    public void hideLoading() {

    }

    @Override
    public void showMessage(@NonNull String message) {
        checkNotNull(message);
        UiUtils.snackbarText(message);
    }

    @Override
    public void launchActivity(@NonNull Intent intent) {
        checkNotNull(intent);
        UiUtils.startActivity(intent);
    }

    @Override
    public void killMyself() {
        finish();
    }


    @Override
    public RxPermissions getRxPermissions() {
        return mRxPermissions;
    }


    @Override
    protected void onDestroy() {
        this.mRxPermissions = null;
        super.onDestroy();
    }
}
