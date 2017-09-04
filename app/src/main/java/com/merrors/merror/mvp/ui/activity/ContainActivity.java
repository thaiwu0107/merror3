package com.merrors.merror.mvp.ui.activity;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentManager;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.jess.arms.base.BaseActivity;
import com.jess.arms.di.component.AppComponent;
import com.jess.arms.utils.UiUtils;
import com.merrors.merror.app.arouter.ARouterPaths;
import com.merrors.merror.di.component.DaggerContainComponent;
import com.merrors.merror.di.module.ContainModule;
import com.merrors.merror.mvp.contract.ContainContract;
import com.merrors.merror.mvp.presenter.ContainPresenter;
import com.merrors.merror.R;
import static com.jess.arms.utils.Preconditions.checkNotNull;

@Route(path = ARouterPaths.ContainActivity)
public class ContainActivity extends BaseActivity<ContainPresenter> implements ContainContract.View {


    @Override
    public void setupActivityComponent(AppComponent appComponent) {
        DaggerContainComponent //如找不到该类,请编译一下项目
                .builder()
                .appComponent(appComponent)
                .containModule(new ContainModule(this))
                .build()
                .inject(this);
    }

    @Override
    public int initView(Bundle savedInstanceState) {
        return R.layout.activity_contain; //如果你不需要框架帮你设置 setContentView(id) 需要自行设置,请返回 0
    }

    @Override
    public void initData(Bundle savedInstanceState) {
        mPresenter.initFragments();
    }

    @Override
    public FragmentManager getSPFragmentManager() {
        return getSupportFragmentManager();
    }

    @Override
    public int getFrameID() {
        return R.id.containFrame;
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


}
