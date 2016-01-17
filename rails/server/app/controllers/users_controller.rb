class UsersController < ApplicationController
  before_action :set_user, only: [:edit, :update, :destroy]

  # POST /users
  # POST /users.json
  def create
    @user = User.new
    user.timeline_token = user_params[:timeline_token]
    user.anilist_token = user.get_refresh_token(user_params[:anilist_token])

    respond_to do |format|
      if @user.save
        send_anime_pins(@user)
        format.html { redirect_to @user, notice: 'User was successfully created.' }
        format.json { render :show, status: :created, location: @user }
      else
        format.html { render :new }
        format.json { render json: @user.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /users/1
  # PATCH/PUT /users/1.json
  def update
    respond_to do |format|
      if @user.update(user_params)
        format.html { redirect_to @user, notice: 'User was successfully updated.' }
        format.json { render :show, status: :ok, location: @user }
      else
        format.html { render :edit }
        format.json { render json: @user.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /users/1
  # DELETE /users/1.json
  def destroy
    @user.destroy
    respond_to do |format|
      format.html { redirect_to users_url, notice: 'User was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_user
      @user = User.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def user_params
       # params.require(:user).permit(:timeline_token, :anilist_token)
       params.permit(:timeline_token, :anilist_token)
    end
end
